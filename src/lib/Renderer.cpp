#include <omni/Renderer.h>

#include <QPainter>

#include <omni/visual/util.h>
#include <omni/visual/Session.h>
#include <omni/visual/Tuning.h>

namespace omni
{
  Renderer::Renderer(Session& _session, RenderOptions const& _options) :
    session_(_session),
    options_(_options)
  {

  }

  RenderOptions const& Renderer::options() const
  {
    return options_;
  }

  RenderOptions& Renderer::options()
  {
    return options_;
  }

  void Renderer::setOptions(RenderOptions const& _options)
  {
    options_ = _options;
  }
    
  void Renderer::renderToFile(QString const& _filename)
  {
    std::vector<QImage> _tuningImages;
    for (auto& _tuning : session_.tunings())
    {
      QImage _image;
      render(_tuning.get(),_image);
      _tuningImages.push_back(_image);
    }

    switch(options_.separationMode())
    {
      case SeparationMode::NONE:
      break;
      case SeparationMode::SCREENS:
      break;
      case SeparationMode::PROJECTORS:
      break;
    };
  }

  void Renderer::render(proj::Tuning const* _tuning, RenderBuffer& _buffer)
  {
    int _w = _tuning->width();
    int _h = _tuning->height();

    /// 1st Step: Render projectors view to texture
    RenderBuffer _projBuffer(_w,_h);

    visual::Session _sessionViz(session_);

    renderToBuffer(_projBuffer,
                   // Projection operation
                   [&](QOpenGLFunctions& _)
    {
      glMultMatrixf(_tuning->projector().projectionMatrix().constData());
    },
    // Model view operation
    [&](QOpenGLFunctions& _)
    {
      // Draw canvas with 
      _sessionViz.update();
      _sessionViz.drawCanvas(options_.mappingOutputMode());
    }
    );
 
    GLuint _projTex = 0;
    
    // 2nd step: Update render buffer as floating point texture
    visual::with_current_context([&](QOpenGLFunctions& _)
    {
      _.glGenTextures(1, &_projTex);
      _.glBindTexture(GL_TEXTURE_2D, _projTex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 
        _projBuffer.width(), _projBuffer.height(), 0, 
        GL_RGBA, GL_FLOAT, _projBuffer.ptr()); 
      _.glBindTexture(GL_TEXTURE_2D, 0);
    });

    // 3rd step: Render warp grid
    RenderBuffer _warpBuffer(_w,_h);

    visual::Tuning _tuningViz(*const_cast<proj::Tuning*>(_tuning));
    _tuningViz.update();

    renderToBuffer(_warpBuffer,
                   // Projection operation
                   [&](QOpenGLFunctions& _)
    {
      QMatrix4x4 _m;
      _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
      glMultMatrixf(_m.constData());
    },
    // Model view operation
    [&](QOpenGLFunctions& _)
    {
      _.glBindTexture(GL_TEXTURE_2D, _projTex);
      _tuningViz.drawWarpPatch();
      _.glBindTexture(GL_TEXTURE_2D, 0);
    });

    // 4th Step: Render blend mask
    RenderBuffer _blendBuffer(_w,_h);
    renderToBuffer(_blendBuffer,
                   // Projection operation
                   [&](QOpenGLFunctions& _)
    {
      QMatrix4x4 _m;
      _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
      glMultMatrixf(_m.constData());
    },
    // Model view operation
    [&](QOpenGLFunctions& _)
    {    
      _.glDisable(GL_TEXTURE_2D);
      glColor4f(1.0,1.0,1.0,1.0);
      _tuningViz.drawBlendMask();
    });

    // 5th step: Merge blend and warp buffer
    _buffer.resize(_w,_h);
    for (int i = 0; i < _buffer.data().size(); ++i)
    {
      auto& _wP = _warpBuffer[i]; // Pixel from warp grid buffer
      _buffer[i] = RGBAFloat(_wP.r,_wP.g,_wP.b,_blendBuffer[i].r);
    }
  }

  void Renderer::render(proj::Tuning const* _tuning, QImage& _image)
  {
    int _w = _tuning->width();
    int _h = _tuning->height();
    
    RenderBuffer _buffer;
    render(_tuning,_buffer);
    _image = QImage(_w,_h*3,QImage::Format_RGB32);

    if (options_.mappingOutputMode() == Mapping::MAPPED_INPUT)
    {
      bufferToImage(_buffer,_image,[&](RGBAFloat const& _pixel)
      {
        return _pixel;
      });
    } else
    {
      QImage _upper8bit(_w,_h,QImage::Format_ARGB32);
      getUpper8bit(_buffer,_upper8bit);
      QImage _lower8bit(_w,_h,QImage::Format_ARGB32);
      getLower8bit(_buffer,_lower8bit);
      QImage _blendMask(_w,_h,QImage::Format_ARGB32);
      getAlphaMask(_buffer,_blendMask);

      QPainter _p(&_image);
      _p.drawImage(QPoint(0,0),_upper8bit);
      _p.drawImage(QPoint(0,_h),_lower8bit);
      _p.drawImage(QPoint(0,2*_h),_blendMask);
      _p.end();
    }

  }

  template<typename PROJECTION, typename MODELVIEW>
  void Renderer::renderToBuffer(RenderBuffer& _buffer, PROJECTION _proj, MODELVIEW _mv)
  {
    int _w = _buffer.width();
    int _h = _buffer.height();

    visual::with_current_context([&](QOpenGLFunctions& _)
    {
      GLuint fb = 0, _colorTex = 0, _depthRb = 0;

      //RGBA8 2D texture, 24 bit depth texture, 256x256
      _.glGenTextures(1, &_colorTex);
      _.glBindTexture(GL_TEXTURE_2D, _colorTex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      //NULL means reserve texture memory, but texels are undefined
      _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _w, _h, 0, GL_RGBA, GL_FLOAT, NULL);

      //-------------------------
      _.glGenFramebuffers(1, &fb);
      _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);
      //   Attach 2D texture to this FBO
      _.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _colorTex, 0);
      //-------------------------
      _.glGenRenderbuffers(1, &_depthRb);
      _.glBindRenderbuffer(GL_RENDERBUFFER_EXT, _depthRb);
      _.glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _w, _h);

      //-------------------------
      //Attach depth buffer to FBO
      _.glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _depthRb);
      //-------------------------
      //Does the GPU support current FBO configuration?
      GLenum status;
      status = _.glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
      switch(status)
      {
      case GL_FRAMEBUFFER_COMPLETE_EXT:
        qDebug() <<"good";
        break;
      default:
        qDebug() << "bad";
      }

      //-------------------------
      //and now you can render to GL_TEXTURE_2D
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);
      {
        _.glClearColor(0.0, 0.0, 0.0, 1.0);
        _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //-------------------------
        glViewport(0, 0, _w, _h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        _proj(_); // Projection operation

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        _mv(_); // Model view operation


        //-------------------------
        _.glReadPixels(0, 0, _w, _h, GL_RGBA, GL_FLOAT, _buffer.ptr());
      }
      _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

      //Delete resources
      _.glDeleteTextures(1, &_colorTex);
      _.glDeleteRenderbuffers(1, &_depthRb);
      //Bind 0, which means render to back buffer, as a result, fb is unbound
      _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
      _.glDeleteFramebuffers(1, &fb);
      glPopAttrib();
    });
  }



  template<typename OPERATION>
  void Renderer::bufferToImage(RenderBuffer const& _buffer, QImage& _image, OPERATION _f)
  {
    _image = QImage(_buffer.width(),_buffer.height(),QImage::Format_ARGB32);

    int _pos = 0;

    for (int y = 0; y < _image.height(); ++y)
    {
      uchar* _line = _image.scanLine(y);
      for (int x = 0; x < _image.width()*4; x += 4)
      {
        RGBAFloat _pixel = _f(_buffer.data()[_pos+x/4]);
        _line[x  ] = qBound(0,int(_pixel.r * 255),255);
        _line[x+1] = qBound(0,int(_pixel.g * 255),255);
        _line[x+2] = qBound(0,int(_pixel.b * 255),255);
        _line[x+3] = qBound(0,int(_pixel.a * 255),255);
      }
      _pos += _image.width();
    }
  }

  void Renderer::getUpper8bit(RenderBuffer const& _buffer, QImage& _image)
  {
    bufferToImage(_buffer,_image,[&](RGBAFloat const& _pixel)
    {
      return RGBAFloat(_pixel.r,_pixel.g,_pixel.b,1.0);
    });
  }

  void Renderer::getLower8bit(RenderBuffer const& _buffer, QImage& _image)
  {
    bufferToImage(_buffer,_image,[&](RGBAFloat const& _pixel)
    {
      return RGBAFloat(
               (int(_pixel.r * 65536.0) & 255) / 255.0,
               (int(_pixel.g * 65536.0) & 255) / 255.0,
               (int(_pixel.b * 65536.0) & 255) / 255.0);
    });
  }

  void Renderer::getAlphaMask(RenderBuffer const& _buffer, QImage& _image)
  {
    bufferToImage(_buffer,_image,[&](RGBAFloat const& _pixel)
    {
      return RGBAFloat(_pixel.a,_pixel.a,_pixel.a);
    });
  }





}

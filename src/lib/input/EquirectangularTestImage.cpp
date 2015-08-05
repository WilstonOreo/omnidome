#include <omni/input/EquirectangularTestImage.h>

#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <omni/util.h>
#include <omni/visual/util.h>

namespace omni
{
  namespace input
  {
    using visual::with_current_context;

    EquirectangularTestImage::EquirectangularTestImage() :
      rulerPos_(-1.0,-1.0)
    {
    }

    EquirectangularTestImage::~EquirectangularTestImage()
    {
    }

    void EquirectangularTestImage::bind(mapping::Interface* _mapping)
    {
      if (!framebuffer_) return;

      with_current_context([this](QOpenGLFunctions& _)
      {
        _.glEnable(GL_TEXTURE_2D);
        _.glBindTexture(GL_TEXTURE_2D, framebuffer_->texture());
      });

      if (_mapping)
      {
        _mapping->bind();
      }
    }

    void EquirectangularTestImage::release(mapping::Interface* _mapping)
    {
      if (_mapping) _mapping->release();

      with_current_context([this](QOpenGLFunctions& _)
      {
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });
    }

    void EquirectangularTestImage::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      bool _changed = rulerPosChanged_;

      if (!framebuffer_)
      {
        QOpenGLFramebufferObjectFormat _format;
        _format.setMipmap(true);

        framebuffer_.reset(new QOpenGLFramebufferObject(4096,2048,_format));
        _changed = true;

        framebuffer_->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

        glBindTexture(GL_TEXTURE_2D, framebuffer_->texture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      if (!shader_)
      {
        static QString _vertSrc = util::fileToStr(":/shaders/test_image.vert");
        static QString _fragmentSrc = util::fileToStr(":/shaders/test_image.frag");
        shader_.reset(new QOpenGLShaderProgram());
        shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
        shader_->link();
        _changed = true;
      }

      if (!_changed) return;

      id_ = 0;
      visual::draw_on_framebuffer(framebuffer_,
      [&](QOpenGLFunctions& _) // Projection Operation
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
        glMultMatrixf(_m.constData());
      },
      [&](QOpenGLFunctions& _) // Model View Operation
      { 
        shader_->bind();
        {
          shader_->setUniformValue("ruler_pos",rulerPos_ - QPointF(0.5,0.5));
          glBegin(GL_QUADS);
          {
            glTexCoord2f(0.0f,0.0f);
            glVertex2f(-0.5f,-0.5f);
            glTexCoord2f(1.0f,0.0f);
            glVertex2f(0.5f,-0.5f);
            glTexCoord2f(1.0f,1.0f);
            glVertex2f(0.5f,0.5f);
            glTexCoord2f(0.0f,1.0f);
            glVertex2f(-0.5f,0.5f);
          }
          glEnd();
        }
        shader_->release();
      });
      
      rulerPosChanged_ = false;
    }

    void EquirectangularTestImage::free()
    {
      framebuffer_.reset();
      shader_.reset();
    }
      
    QPointF EquirectangularTestImage::rulerPos() const
    {
      return rulerPos_;
    }
    
    void EquirectangularTestImage::setRulerPos(QPointF const& _rulerPos) 
    {
      rulerPos_ = _rulerPos;
      rulerPosChanged_ = true;
    }
 
    QSize EquirectangularTestImage::size() const 
    {
      return QSize(4096,2048);
    }

    

    void EquirectangularTestImage::toStream(QDataStream& _stream) const
    {
      _stream << rulerPos_;
    }

    void EquirectangularTestImage::fromStream(QDataStream& _stream)
    {
      _stream >> rulerPos_;
      update();
    }
  }
}

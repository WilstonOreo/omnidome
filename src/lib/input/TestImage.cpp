#include <omni/input/TestImage.h>

#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <omni/visual/util.h>
#include <omni/util.h>

namespace omni
{
  namespace input 
  {
    TestImage::TestImage() :
      rulerPos_(-1.0,-1.0)
    {
    }

    TestImage::~TestImage()
    {
      free();
    }
      
    GLuint TestImage::textureId() const
    {
      return framebuffer_->texture();
    }

    void TestImage::free()
    {
      shader_.reset();
      framebuffer_.reset();
    }

    void TestImage::setRulerPos(QPointF const& _rulerPos) 
    {
      rulerPos_ = _rulerPos;
      rulerPosChanged_ = true;
    }

    QPointF TestImage::rulerPos() const
    {
      return rulerPos_;
    }
      
    void TestImage::shaderUniformHandler()
    {
      shader_->setUniformValue("ruler_pos",rulerPos_ - QPointF(0.5,0.5));
    }

    void TestImage::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      bool _changed = rulerPosChanged_;

      if (!framebuffer_)
      {
        QOpenGLFramebufferObjectFormat _format;
        _format.setMipmap(true);

        auto _size = size();
        framebuffer_.reset(new QOpenGLFramebufferObject(
              _size.width(),
              _size.height(),_format));
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
          shaderUniformHandler();
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


      if (!shader_)
      { 
        shader_.reset(new QOpenGLShaderProgram());
        shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource());
        shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource());
        shader_->link();
      }
    }
      
    void TestImage::toStream(QDataStream& _stream) const
    {
      _stream << rulerPos_;
    }

    void TestImage::fromStream(QDataStream& _stream)
    { 
      _stream >> rulerPos_;
      update();
    }
  }
}


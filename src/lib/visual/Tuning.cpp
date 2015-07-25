#include <omni/visual/Tuning.h>

#include <QOpenGLTexture>
#include <omni/util.h>

namespace omni
{
  namespace visual
  {
    std::unique_ptr<QOpenGLShaderProgram> Tuning::blendShader_;
    
    Tuning::Tuning(omni::proj::Tuning& _tuning) :
      tuning_(_tuning)
    {
    }

    omni::proj::Tuning const& Tuning::tuning() const
    {
      return tuning_;
    }

    void Tuning::drawWarpGrid() const
    {
      

    }

    void Tuning::update()
    {
      if (!blendShader_)
      {
        static QString _vertSrc = util::fileToStr(":/shaders/blend.vert");
        static QString _fragmentSrc = util::fileToStr(":/shaders/blend.frag");
        blendShader_.reset(new QOpenGLShaderProgram());
        blendShader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        blendShader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
      }
      updateBlendTexture();
    }

    void Tuning::updateBlendTexture()
    {
      if (!blendTex_)
      {
        blendTex_.reset(new QOpenGLTexture(QOpenGLTexture::Target2D));
      }
    }

    void Tuning::drawBlendMask() const
    {
      if (!blendShader_) return;

      auto& _mask = tuning().blendMask();
      glEnable(GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0,1.0,1.0,1.0);

      blendShader_->bind();
      {
        blendShader_->setUniformValue("top",_mask.topWidth());
        blendShader_->setUniformValue("right",_mask.rightWidth());
        blendShader_->setUniformValue("bottom",_mask.bottomWidth());
        blendShader_->setUniformValue("left",_mask.leftWidth());
        blendShader_->setUniformValue("gamma",_mask.gamma());
        blendShader_->setUniformValue("mask",GLfloat(-1.0));
        drawWarpGrid();
      }
      blendShader_->release();

      glDisable(GL_BLEND);
      glColor4f(0.0,0.0,0.0,1.0);

      glBegin(GL_QUADS);
      float _b = 4.0;
      glVertex2f(-0.5,-0.5 - _b);
      glVertex2f(0.5-_b,-0.5);
      glVertex2f(0.5-_b,0.5);
      glVertex2f(-0.5,0.5 + _b);

      glVertex2f(-0.5 + _b,-0.5 - _b);
      glVertex2f(0.5,-0.5);
      glVertex2f(0.5,0.5);
      glVertex2f(-0.5 + _b,0.5 + _b);

      glVertex2f(-0.5 + _b,0.5 + _b);
      glVertex2f(0.5 - _b,0.5 + _b);
      glVertex2f(0.5 - _b,0.5);
      glVertex2f(-0.5 + _b,0.5);

      glVertex2f(-0.5 + _b,-0.5);
      glVertex2f(0.5 - _b,-0.5);
      glVertex2f(0.5 - _b,-0.5 - _b);
      glVertex2f(-0.5 + _b,-0.5 - _b);

      glEnd();

      glEnable(GL_BLEND);
      /*
          use_texture(*strokeBlendTex_,[&]()
        {
          glColor4f(0.0,0.0,0.0,1.0);
          glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          drawScreenRect();
        });

      */
    }

  }
}

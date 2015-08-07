#include <omni/visual/Session.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <omni/util.h>
#include <omni/visual/util.h>
#include <omni/proj/Frustum.h>

namespace omni
{
  namespace visual
  {
    std::unique_ptr<QOpenGLShaderProgram> Session::frustumShader_;

    Session::Session(omni::Session& _session) : 
      session_(_session)
    {
    }
    
    omni::Session& Session::session()
    {
      return session_;
    }
      
    omni::Session const& Session::session() const
    {
      return session_;
    }
      
    void Session::drawCanvas() const
    {
      auto _canvas = session_.canvas();

      if (!_canvas) return; 

      with_current_context([&](QOpenGLFunctions& _)
      {
        auto* _input = session_.inputs().current();
        auto* _mapping = session_.mapping();

        if (session_.hasOutput())
        {
          GLuint _texId = _input ? _input->textureId() : 0;
          _.glEnable(GL_TEXTURE_2D);

          _mapping->bind();
          _.glBindTexture(GL_TEXTURE_2D, _texId);
          _canvas->draw();

          _.glBindTexture(GL_TEXTURE_2D, 0);

          _mapping->release();
        } else
        {
          // Render canvas with lighting when there is no input
          glColor4f(1.0,1.0,1.0,1.0);
          _.glEnable(GL_LIGHTING);
          _canvas->draw();
          glPopMatrix();
          _.glDisable(GL_LIGHTING);
        }
      });
    }
      
    void Session::drawCanvasWithFrustumIntersections() const
    {
      auto _canvas = session_.canvas();
      if (!frustumShader_ || !_canvas) return;

        frustumShader_->bind();
        for (auto& _tuning : session_.tunings())
        {
          proj::Projector _proj = _tuning->projector();

          auto _m = _canvas->matrix().inverted() * _proj.matrix();
          auto _rot = _canvas->matrix().inverted();
          _rot.setColumn(3,QVector4D(0,0,0,1));
 
          auto _c = _tuning->color();
          frustumShader_->setUniformValue("color",_c.redF(),_c.greenF(),_c.blueF());

          qreal _a = _proj.fov().radians() *0.5;
          qreal _height = tan(_a);
          qreal _width = _height * _proj.aspectRatio();

          QVector3D eye_ = _m.column(3).toVector3D();
          QVector3D topLeft_ = _m * QVector3D(1.0,-_width,_height) - eye_;
          QVector3D topRight_ = _m * QVector3D(1.0,_width,_height) - eye_;
          QVector3D bottomLeft_ = _m * QVector3D(1.0,-_width,-_height) - eye_;
          QVector3D bottomRight_ = _m * QVector3D(1.0,_width,-_height) - eye_;
          QVector3D lookAt_ = _m * QVector3D(1.0,0.0,0.0) - eye_;

          // Setup frustum uniforms for intersection test
          frustumShader_->setUniformValue("eye",eye_); // *_rot - _m.column(3).toVector3D());
          frustumShader_->setUniformValue("look_at",lookAt_);
          frustumShader_->setUniformValue("top_left",topLeft_);
          frustumShader_->setUniformValue("top_right",topRight_);
          frustumShader_->setUniformValue("bottom_left",bottomLeft_);
          frustumShader_->setUniformValue("bottom_right",bottomRight_);
          frustumShader_->setUniformValue("matrix",_rot);

          glDisable(GL_DEPTH_TEST);
          _canvas->draw(); 
          glEnable(GL_DEPTH_TEST);
        }

        frustumShader_->release();
    }

    void Session::drawProjectors() const
    {
          glDisable(GL_DEPTH_TEST);
      for (auto& _proj : projectors_)
      {
        _proj.draw();
      }

      if (session_.canvas())
      {
        for (auto& _proj : projectors_)
          _proj.drawPositioning(session_.canvas()->center());
      }
      glEnable(GL_DEPTH_TEST);
    }
      
    void Session::drawProjectorHalos() const
    {
      for (auto& _proj : projectors_)
      {
        _proj.drawHalo();
      }
    }

    void Session::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      auto* _canvas = session_.canvas();
      if (!_canvas) return;

      _canvas->update();

      auto* _input = session_.inputs().current();
      if (_input) _input->update();

      // Update projector visualizers
      projectors_.clear();
      for (int i = 0; i < session_.tunings().size(); ++i)
      {
        projectors_.emplace_back(session_.tunings()[i]->projector());
        projectors_.back().setColor(session_.tunings()[i]->color());
        projectors_.back().setSelected(i == session_.tunings().currentIndex());
        projectors_.back().update();
      }

      // Setup frustum/canvas intersection shader
      if (!frustumShader_)
      {
        using omni::util::fileToStr;
        static QString _vertSrc = fileToStr(":/shaders/frustum.vert");
        static QString _fragmentSrc = fileToStr(":/shaders/frustum.frag");
        
        frustumShader_.reset(new QOpenGLShaderProgram());
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
        frustumShader_->link();
      }
    }

    void Session::free()
    {
      auto* _input = session_.inputs().current();
      if (_input) _input->free();
    }
  }
}

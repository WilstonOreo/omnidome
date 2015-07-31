#include <omni/visual/Session.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <omni/util.h>
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

      auto* _input = session_.inputs().current();
      auto* _mapping = session_.mapping();

      if (_input) _input->bind(_mapping);

      _canvas->draw();

      if (_input) _input->release(_mapping);
    }
      
    void Session::drawCanvasWithFrustumIntersections() const
    {
      auto _canvas = session_.canvas();
      if (!frustumShader_ || !_canvas) return;

      frustumShader_->bind();
      for (auto& _tuning : session_.tunings())
      {
        proj::Frustum _f(_tuning->projector());

        auto _c = _tuning->color();
        frustumShader_->setUniformValue("color",_c.redF(),_c.greenF(),_c.blueF());
        
        // Setup frustum uniforms for intersection test
        frustumShader_->setUniformValue("eye",_f.eye());
        frustumShader_->setUniformValue("look_at",_f.lookAt());
        frustumShader_->setUniformValue("top_left",_f.topLeft());
        frustumShader_->setUniformValue("top_right",_f.topRight());
        frustumShader_->setUniformValue("bottom_left",_f.bottomLeft());
        frustumShader_->setUniformValue("bottom_right",_f.bottomRight());
        _canvas->draw(); 
      }

      frustumShader_->release();
    }

    void Session::drawProjectors() const
    {
      for (auto& _proj : projectors_)
      {
        _proj.draw();
      }

      if (session_.canvas())
      {
        for (auto& _proj : projectors_)
          _proj.drawPositioning(session_.canvas()->center());
      }
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
        static QString _vertSrc = util::fileToStr(":/shaders/frustum.vert");
        static QString _fragmentSrc = util::fileToStr(":/shaders/frustum.frag");
        
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

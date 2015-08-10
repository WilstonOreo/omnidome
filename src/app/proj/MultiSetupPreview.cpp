#include <omni/ui/proj/MultiSetupPreview.h>

#include <omni/visual/util.h>
#include <omni/visual/Projector.h>
#include <omni/proj/PeripheralSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      MultiSetupPreview::MultiSetupPreview(QWidget* _parent) :
        GLView3D(_parent)
      {
      }

      MultiSetupPreview::~MultiSetupPreview()
      {
      }

      omni::proj::MultiSetup* MultiSetupPreview::multiSetup()
      {
        return multiSetup_;
      }

      omni::proj::MultiSetup const* MultiSetupPreview::multiSetup() const
      {
        return multiSetup_;
      }
      void MultiSetupPreview::setMultiSetup(omni::proj::MultiSetup* _multiSetup)
      {
        multiSetup_ = _multiSetup;
        updateProjectors();
      }
        
      void MultiSetupPreview::updateProjectors()
      {
        if (!multiSetup_) return;

        projectorViz_.clear();

        projectors_ = multiSetup_->projectors();
        for (auto& _projector : projectors_)
        { 
          visual::Projector _projViz(_projector);
          _projViz.setColor("#00ccff");
          /// Make new projector visualizer
          projectorViz_.emplace_back(_projViz);
        }
        update();
      }

      void MultiSetupPreview::paintGL()
      {
        if (!session()) return;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        visual::viewport(this);

        this->setupCamera();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        updateLight();

        visual::with_current_context([&](QOpenGLFunctions& _)
        {
          auto* _canvas = session()->canvas();
          
          _.glEnable(GL_LIGHTING);
          _.glEnable(GL_DEPTH_TEST);
          if (_canvas)
          {
            glColor4f(1.0,1.0,1.0,1.0);
            _canvas->draw();
          }
          
          _.glDisable(GL_DEPTH_TEST);
          _.glDisable(GL_LIGHTING);
          for (auto& _projector : projectorViz_) 
          {
            _projector.draw();
            _projector.drawPositioning(_canvas ? _canvas->center() : QVector3D(0,0,0));
          }

          for (auto& _projector : projectors_)
            session_->drawFrustumIntersection(_projector,"#00ccff");
          
          _.glEnable(GL_DEPTH_TEST);

          for (auto& _projector : projectorViz_)
            _projector.drawHalo();
        });
      }

    }
  }
}


#include <omni/ui/proj/MultiSetupPreview.h>

#include <omni/visual/util.h>

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

      omni::proj::MultiSetup* MultiSetupPreview::getMultiSetup()
      {
        return template_;
      }

      omni::proj::MultiSetup const* MultiSetupPreview::getMultiSetup() const
      {
        return template_;
      }
      void MultiSetupPreview::setMultiSetup(omni::proj::MultiSetup* _template)
      {
        template_ = _template;
      }

      void MultiSetupPreview::paintGL()
      {
        if (!session()) return;

        doneCurrent();
        this->session_->update();
        makeCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        visual::viewport(this);

        this->setupCamera();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        updateLight();
        if (session()->canvas())
          session()->canvas()->draw();

        //this->session_->drawProjectors();

        this->session_->drawCanvasWithFrustumIntersections();
        // this->session_->drawProjectorHalos();
      }

    }
  }
}


#include <omni/ui/proj/TemplatePreview.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TemplatePreview::TemplatePreview(QWidget* _parent) :
        GLView3D(_parent)
      {
      }

      TemplatePreview::~TemplatePreview()
      {
      }

      omni::proj::Template* TemplatePreview::getTemplate()
      {
        return template_;
      }

      omni::proj::Template const* TemplatePreview::getTemplate() const
      {
        return template_;
      }
      void TemplatePreview::setTemplate(omni::proj::Template* _template)
      {
        template_ = _template;
      }

      void TemplatePreview::paintGL()
      {
        if (!session()) return;

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


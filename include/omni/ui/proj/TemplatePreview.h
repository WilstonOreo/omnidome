#ifndef OMNI_UI_PROJ_TEMPLATEPREVIEW_H
#define OMNI_UI_PROJ_TEMPLATEPREVIEW_H

#include <omni/ui/GLView3D.h>
#include <omni/proj/Template.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class TemplatePreview : public GLView3D
      {
        Q_OBJECT
      public:
        TemplatePreview(QWidget* _parent);
        ~TemplatePreview();

        omni::proj::Template* getTemplate();
        omni::proj::Template const* getTemplate() const;
        void setTemplate(omni::proj::Template* _template);


      protected:
        void paintGL();
 
      private:
        visual::Camera camera_;
        std::array<visual::Light,3> lights_;
        omni::proj::Template* template_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TEMPLATEPREVIEW_H */

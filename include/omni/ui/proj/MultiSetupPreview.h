#ifndef OMNI_UI_PROJ_MULTISETUPPREVIEW_H
#define OMNI_UI_PROJ_MULTISETUPPREVIEW_H

#include <omni/ui/GLView3D.h>
#include <omni/proj/MultiSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class MultiSetupPreview : public GLView3D
      {
        Q_OBJECT
      public:
        MultiSetupPreview(QWidget* _parent);
        ~MultiSetupPreview();

        omni::proj::MultiSetup* multiSetup();
        omni::proj::MultiSetup const* multiSetup() const;
        void setMultiSetup(omni::proj::MultiSetup* _template);

      public slots:

        void updateProjectors();

      protected:
        void paintGL();
 
      private:
        visual::Camera camera_;
        std::array<visual::Light,3> lights_;
        omni::proj::MultiSetup* multiSetup_ = nullptr;

        std::vector<omni::proj::Projector> projectors_; 
        std::list<visual::Projector> projectorViz_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUPPREVIEW_H */

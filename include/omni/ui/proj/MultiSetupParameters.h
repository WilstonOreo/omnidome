#ifndef OMNI_UI_MULTISETUPPARAMETERS_H
#define OMNI_UI_MULTISETUPPARAMETERS_H

#include <omni/ui/ParameterWidget.h>
#include <omni/proj/MultiSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class MultiSetupParameters : public ParameterWidget 
      {
        Q_OBJECT
      public: 
        MultiSetupParameters(QWidget* _parent);
        ~MultiSetupParameters();

        omni::proj::MultiSetup* multiSetup();
        omni::proj::MultiSetup const* multiSetup() const;
        void setMultiSetup(omni::proj::MultiSetup* _template);
        
        /// Set parameters from sliders to template
        void updateParameters();
      private:
      
        /// Clear and setup all parameter widgets
        void setup();

        omni::proj::MultiSetup* multiSetup_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_MULTISETUPPARAMETERS_H */

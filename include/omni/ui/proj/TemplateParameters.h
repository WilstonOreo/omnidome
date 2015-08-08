#ifndef OMNI_UI_TEMPLATEPARAMETERS_H
#define OMNI_UI_TEMPLATEPARAMETERS_H

#include <omni/ui/ParameterWidget.h>
#include <omni/proj/Template.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class TemplateParameters : public ParameterWidget 
      {
        Q_OBJECT
      public: 
        TemplateParameters(QWidget* _parent);
        ~TemplateParameters();

        omni::proj::Template* getTemplate();
        omni::proj::Template const* getTemplate() const;
        void setTemplate(omni::proj::Template* _template);
        
        /// Set parameters from sliders to template
        void updateParameters();
      private:
      
        /// Clear and setup all parameter widgets
        void setup();

        omni::proj::Template* template_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_TEMPLATEPARAMETERS_H */

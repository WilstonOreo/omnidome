#ifndef OMNI_UI_MAPPINGPARAMETERS_H_
#define OMNI_UI_MAPPINGPARAMETERS_H_

#include <omni/mapping/Interface.h>
#include <omni/ui/ParameterWidget.h>

namespace omni
{
  namespace ui
  {
    /**@brief Parameter widget for getting and setting mapping parameters 
     **/
    class MappingParameters : public ParameterWidget
    {
      Q_OBJECT
    public:
      MappingParameters(QWidget* _parent);
      ~MappingParameters();

      mapping::Interface* mapping();
      mapping::Interface const* mapping() const;
      void setMapping(mapping::Interface* _mapping);

      /// Set parameters from sliders to mapping
      void updateParameters();
    private:
      slim::Rotation* rotation_ = nullptr;

      /// Show widgets parameter according to given mapping type
      void widgetVisibility();
      
      /// Clear and setup all parameter widgets
      void setup();

      mapping::Interface* mapping_ = nullptr;
    };
  }
}


#endif /* OMNI_UI_MAPPINGPARAMETERS_H_ */

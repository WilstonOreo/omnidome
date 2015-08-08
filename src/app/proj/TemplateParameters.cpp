#include <omni/ui/proj/TemplateParameters.h>

#include <omni/proj/RingArray.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TemplateParameters::TemplateParameters(QWidget* _parent) : 
        ParameterWidget(_parent)
      {
      }

      TemplateParameters::~TemplateParameters() 
      {
      }

      omni::proj::Template* TemplateParameters::getTemplate() 
      {
        return template_;
      }
      
      omni::proj::Template const* TemplateParameters::getTemplate() const 
      {
        return template_;
      }
    
      void TemplateParameters::setTemplate(omni::proj::Template* _template)
      {
        template_ = _template;
        setup();
      }

      void TemplateParameters::updateParameters()
      {
        if (!template_ || isLocked()) return;

          
        if (template_->getTypeId() == "RingArray")
        {

        }
      
        emit parametersUpdated();
      }

      void TemplateParameters::setup()
      {
        clear();
        if (!template_) return;

        this->locked([&]() 
        {
          if (template_->getTypeId() == "RingArray")
          { 
            auto* _ringArray = static_cast<omni::proj::RingArray*>(template_);

            addIntegerWidget("Projectors",_ringArray->numberOfProjectors(),2,10);
            addOffsetWidget("Distance To Center",_ringArray->distanceCenter(),0.1,10.0);
            addOffsetWidget("Tower Height",_ringArray->towerHeight(),-2.0,2.0);
            addAngleWidget("Pitch",_ringArray->pitch().degrees(),-90.0,90.0);
            addAngleWidget("Yaw",_ringArray->yaw().degrees(),-180.0,180.0);
            addAngleWidget("FOV",_ringArray->fov().degrees(),30.0,100.0);
          }
        });
      }
    }
  }
}

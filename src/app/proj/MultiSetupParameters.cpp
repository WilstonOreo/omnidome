#include <omni/ui/proj/MultiSetupParameters.h>

#include <QVBoxLayout>
#include <omni/proj/RingArray.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      MultiSetupParameters::MultiSetupParameters(QWidget* _parent) : 
        ParameterWidget(_parent)
      {
        setLayout(new QVBoxLayout);
      }

      MultiSetupParameters::~MultiSetupParameters() 
      {
      }

      omni::proj::MultiSetup* MultiSetupParameters::multiSetup() 
      {
        return multiSetup_;
      }
      
      omni::proj::MultiSetup const* MultiSetupParameters::multiSetup() const 
      {
        return multiSetup_;
      }
    
      void MultiSetupParameters::setMultiSetup(omni::proj::MultiSetup* _template)
      {
        multiSetup_ = _template;
        setup();
      }

      void MultiSetupParameters::updateParameters()
      {
        if (!multiSetup_ || isLocked()) return;

          
        if (multiSetup_->getTypeId() == "RingArray")
        {
          auto* _ringArray = static_cast<omni::proj::RingArray*>(multiSetup_);

          _ringArray->setNumberOfProjectors(getParamAsInt("Projectors"));
          _ringArray->setDistanceCenter(getParamAsFloat("Distance To Center"));
          _ringArray->setTowerHeight(getParamAsFloat("Tower Height"));
          _ringArray->setPitch(getParamAsFloat("Pitch"));
          _ringArray->setYaw(getParamAsFloat("Yaw"));
          _ringArray->setFov(getParamAsFloat("FOV"));
        }
      
        emit parametersUpdated();
      }

      void MultiSetupParameters::setup()
      {
        clear();
        if (!multiSetup_) return;

        this->locked([&]() 
        {
          if (multiSetup_->getTypeId() == "RingArray")
          { 
            auto* _ringArray = static_cast<omni::proj::RingArray*>(multiSetup_);

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

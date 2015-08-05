#ifndef OMNI_PROJ_PERIPHERALSETUP_H_
#define OMNI_PROJ_PERIPHERALSETUP_H_

#include "AngleSetup.h"

namespace omni
{
  namespace proj
  {
    class PeripheralSetup : 
      public AngleSetup,
      SetupRegistrar<PeripheralSetup>
    {
    public:
      OMNI_TYPEID("PeripheralSetup")

      PeripheralSetup();
      
      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);
      void setup(Projector&);
 
      Angle deltaYaw() const;
      void setDeltaYaw(Angle);

      qreal distanceCenter() const;
      void setDistanceCenter(qreal);

      qreal towerHeight() const;
      void setTowerHeight(qreal);

      qreal shift() const;
      void setShift(qreal);

    private:
      Angle deltaYaw_;
      qreal distanceCenter_;
      qreal towerHeight_;
      qreal shift_;
    };    
  }
}

#endif /* OMNI_PROJ_PERIPHERALSETUP_H_ */

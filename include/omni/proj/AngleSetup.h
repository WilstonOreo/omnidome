#ifndef OMNI_PROJ_ANGLESETUP_H_
#define OMNI_PROJ_ANGLESETUP_H_

#include "Setup.h"
#include <omni/EulerAngles.h>

namespace omni
{
  namespace proj
  {
    /// Basic projector setup with three euler angles and no positioning offsets
    class AngleSetup : 
      public Setup,
      public EulerAngles, // Inherit from this to get yaw, pitch, roll angle
      private SetupRegistrar<AngleSetup>
    {
    public:
      OMNI_TYPEID("AngleSetup")

      AngleSetup();
      ~AngleSetup();

      virtual void toStream(QDataStream&) const;
      virtual void fromStream(QDataStream&);
      virtual void setup(Projector&);
    };
  }
}

#endif /* OMNI_PROJ_ANGLESETUP_H_ */

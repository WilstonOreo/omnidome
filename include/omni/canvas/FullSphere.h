#ifndef OMNI_CANVAS_FULLSPHERE_H_
#define OMNI_CANVAS_FULLSPHERE_H_

#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    class FullSphere :
      public Dome,
      Registrar<FullSphere> 
    {
    public:
      OMNI_TYPEID("FullSphere")
      FullSphere();
      ~FullSphere();
    };
  }
}

#endif /* OMNI_DOME_FULLSPHERE_H_ */

#ifndef OMNI_CANVAS_TILTEDDOME_H_
#define OMNI_CANVAS_TILTEDDOME_H_

#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    class TiltedDome :
      public Dome,
      Registrar<TiltedDome> 
    {
    public:
      OMNI_TYPEID("TiltedDome")
      TiltedDome();
      ~TiltedDome();

      void draw() const;
      void drawAux() const;
      MappingModeSet supportedMapModes() const; 

      void update();
    };
  }
}

#endif /* OMNI_DOME_TILTEDDOME_H_ */

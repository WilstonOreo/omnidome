#ifndef OMNI_CANVAS_RADOME_H_
#define OMNI_CANVAS_RADOME_H_

#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    class Radome :
      public Dome,
      Registrar<Radome> 
    {
    public:
      OMNI_TYPEID("Radome")
      Radome();
      ~Radome();

      void draw() const;
      void drawAux() const;

      void update();
    };
  }
}

#endif /* OMNI_DOME_RADOME_H_ */

#ifndef OMNI_CANVAS_INFLATABLEDOME_H_
#define OMNI_CANVAS_INFLATABLEDOME_H_

#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    class InflatableDome : 
      public Dome, 
      Registrar<InflatableDome>
    {
    public: 
      OMNI_TYPEID("InflatableDome")

      InflatableDome();
      ~InflatableDome();

      void draw() const;
      void drawAux() const;
      void update();
    };
  }

  using canvas::InflatableDome;
}

#endif /* OMNI_DOME_INFLATABLE_H_ */

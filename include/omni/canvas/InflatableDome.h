#ifndef OMNI_CANVAS_INFLATABLEDOME_H_
#define OMNI_CANVAS_INFLATABLEDOME_H_

#include <omni/canvas/Dome.h>
#include <omni/canvas/FullSphere.h>

namespace omni
{
  namespace canvas
  {
    /**@brief A fullsphere dome with z offset and squeeze factor
     **/
    class InflatableDome : 
      public FullSphere, 
      Registrar<InflatableDome>
    {
    public: 
      OMNI_TYPEID("InflatableDome")

      InflatableDome();
      ~InflatableDome();
      
      float squeeze() const;
      void setSqueeze(float _squeeze);

      void draw() const;
      void update();

    private:
      float squeeze_ = 1.0;
    };
  }

  using canvas::InflatableDome;
}

#endif /* OMNI_DOME_INFLATABLE_H_ */

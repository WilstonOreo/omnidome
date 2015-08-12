#include <omni/canvas/FullSphere.h>

namespace omni
{
  namespace canvas
  {
    FullSphere::FullSphere() 
    {
      sphere_.setBottom(-1.0);
      setRadius(5.0);
      update();
    }

    FullSphere::~FullSphere()
    {
    }

  }
}

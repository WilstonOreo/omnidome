#ifndef OMNI_VISUAL_CIRCLE_H_
#define OMNI_VISUAL_CIRCLE_H_

#include <omni/visual/Interface.h>

namespace omni
{
  namespace visual
  {
    class Circle
    {
    public:
      Circle();
      ~Circle();

      virtual void draw() const;

    private:
      int numberSegments_ = 24;

    };
  }
}


#endif /* OMNI_VISUAL_CIRCLE_H_ */

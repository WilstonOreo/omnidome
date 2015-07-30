#ifndef OMNI_CANVAS_ENVELOPE_H_
#define OMNI_CANVAS_ENVELOPE_H_

#include <omni/canvas/Interface.h>

namespace omni
{
  namespace canvas
  {
    /// An enveloped canvas can enclose the viewer and can be projected from inside or outside
    class Envelope : 
      public Canvas
    {
    public:
      Envelope();
      ~Envelope();

      /// Projector mode flag determine if projectors are placed in- or outside 
      enum ProjectionMode 
      {
        PROJECT_INSIDE,
        PROJECT_OUTSIDE,
        PROJECT_BOTH_SIDES
      };
      
      /// An Envelope canvas always can have a normal map
      bool hasUVWMap() const;

      ProjectionMode projectionMode() const;
      void setProjectionMode(ProjectionMode);

      /// Returns copy of cached bounding box
      Box bounds() const;

      virtual void draw() const;

    protected:

      /// Protected bounding box member
      omni::Box bounds_;

    private:
      ProjectionMode projectionMode_ = PROJECT_INSIDE;
    };
  }
}

#endif /* OMNI_CANVAS_ENVELOPE_H_ */

#ifndef OMNI_CANVAS_HALFDOME_H_
#define OMNI_CANVAS_HALFDOME_H_

#include <omni/Vertex.h>
#include <omni/canvas/Dome.h>
#include <omni/EulerAngles.h>

namespace omni
{
  namespace canvas
  {
    /// A rotatable half sphere dome
    class HalfDome : 
      public Dome, 
      public Registrar<HalfDome>
    {
    public:
      OMNI_TYPEID("HalfDome")
      HalfDome(qreal _diameter = 10.0);
      ~HalfDome();

      void draw() const;

      EulerAngles& angles();
      EulerAngles const& angles() const;

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

      QMatrix4x4 matrix() const;

    private:
      EulerAngles angles_;
    };
  }
}

#endif /* OMNI_CANVAS_HALFDOME_H_ */

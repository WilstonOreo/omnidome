#ifndef OMNI_CANVAS_DOME_H_
#define OMNI_CANVAS_DOME_H_

#include <omni/Vertex.h>
#include <omni/visual/Sphere.h>
#include <omni/canvas/Envelope.h>

namespace omni
{
  namespace canvas
  {
    /// A basic dome canvas width a radius
    class Dome : public Envelope 
    {
    public:
      Dome();
      virtual ~Dome();

      /// Return radius (10.0m by default)
      qreal radius() const;
      
      /// Set radius in meter
      void setRadius(qreal); 
      
      /// Return diameter in meter
      qreal diameter() const;

      /// Set diameter in meter
      void setDiameter(qreal);

      /// Return internally cached bounding box
      Box bounds() const;

      virtual void update();

      virtual void toStream(QDataStream&) const;
      virtual void fromStream(QDataStream&);
      
    protected:

      visual::Sphere sphere_;
      Box bounds_;
    private:
    };
  }
}

#endif /* OMNI_CANVAS_DOME_H_ */

#ifndef OMNI_CANVAS_DOME_H_
#define OMNI_CANVAS_DOME_H_

#include <omni/Vertex.h>
#include <omni/visual/Sphere.h>
#include <omni/canvas/Envelope.h>

namespace omni
{
  namespace canvas
  {
    /// A basic dome canvas width a radius/diameter and offset
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
      
      /// Return center of sphere
      QVector3D center() const;

      /// Return set center of sphere
      void setCenter(QVector3D const&);

      virtual void draw() const;
      virtual void drawAux() const;

      virtual void update();

      virtual void toStream(QDataStream&) const;
      virtual void fromStream(QDataStream&);
      
      inline virtual UVWSource uvwSource() const 
      {
        return UVWSource::NORMAL;
      }

    protected:

      visual::Sphere sphere_;
      Box bounds_;
    private:
      QVector3D center_;
    };
  }
}

#endif /* OMNI_CANVAS_DOME_H_ */

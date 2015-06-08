#ifndef OMNI_CANVAS_HALFDOME_H_
#define OMNI_CANVAS_HALFDOME_H_

#include <omni/Vertex.h>
#include <omni/canvas/Dome.h>
#include <omni/EulerAngles.h>

namespace omni
{
  namespace canvas
  {
    /// A generic half sphere dome
    class HalfDome : 
      public Dome, 
      Registrar<HalfDome>
    {
    public:
      OMNI_TYPEID("HalfDome")
      HalfDome(qreal _diameter = 10.0);
      ~HalfDome();

      void draw() const;
      void drawAux() const;

      MappingModeSet supportedMapModes() const; 

      EulerAngles& angles();
      EulerAngles const& angles() const;

      QVector3D center() const;
      void setCenter(QVector3D const&);

      void update();

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;
    
    private:
      EulerAngles angles_;
      QVector3D center_;
    };
  }
}

#endif /* OMNI_CANVAS_HALFDOME_H_ */

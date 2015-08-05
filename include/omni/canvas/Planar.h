#ifndef OMNI_CANVAS_PLANAR_H_
#define OMNI_CANVAS_PLANAR_H_

#include <omni/canvas/Interface.h>
#include <omni/EulerAngles.h>

namespace omni
{
  namespace canvas
  {
    /// A planar canvas 
    class Planar : 
      public Canvas,
      Registrar<Planar>
    {
    public:
      OMNI_TYPEID("Planar")

      Planar();
      ~Planar();
      
      void draw() const;
      void drawAux() const;

      float width() const;
      void setWidth(float _width);
      
      float height() const;
      void setHeight(float _height);

      QVector3D center() const;
      void setCenter(QVector3D const&);
      
      EulerAngles& angles();
      EulerAngles const& angles() const;
       
      omni::Box bounds() const;

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

    private:
      float width_ = 1.0, height_ = 1.0;
      QVector3D center_;
      EulerAngles angles_;
    };
  }
}


#endif /* OMNI_CANVAS_PLANAR_H_ */

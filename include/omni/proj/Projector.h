#ifndef OMNI_PROJ_PROJECTOR_H_
#define OMNI_PROJ_PROJECTOR_H_

#include <QMatrix4x4>
#include <omni/Angle.h>
#include "Screen.h"

namespace omni
{
  namespace proj
  {
    class Projector  
    {
    public:
      Projector();
      Projector(
          Screen const* _screen,
          Angle _fov = 45.0);

      qreal aspectRatio() const;
      void setScreen(Screen const* _screen);
      Screen const* screen() const;
    
      qreal throwRatio() const;
      void setThrowRatio(qreal);

      Angle fov() const;
      void setFov(Angle _fov);

      QMatrix4x4& matrix();
      QMatrix4x4 const& matrix() const;
      void setMatrix(QMatrix4x4 const&);

      QVector3D pos() const;

      friend bool operator==(Projector const&,Projector const&);

    private:
      QMatrix4x4 matrix_;
      Screen const* screen_ = nullptr;
      Angle fov_;
    };
  }

  using proj::Projector;
}

QDataStream& operator>>(QDataStream&, omni::proj::Projector&);
QDataStream& operator<<(QDataStream&, omni::proj::Projector const&);

#endif /* OMNI_PROJ_PROJECTOR_H_ */

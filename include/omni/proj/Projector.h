#ifndef OMNI_PROJ_PROJECTOR_H_
#define OMNI_PROJ_PROJECTOR_H_

#include <memory>
#include <QMatrix4x4>
#include <QScreen>
#include <omni/Angle.h>
#include "Screen.h"
#include "Setup.h"

namespace omni
{
  namespace proj
  {
    class Projector  
    {
    public:
      Projector();
      Projector(
          QScreen const* _screen,
          Angle _fov = 45.0);

      /**@brief Make a new projector setup with a certain id and delete old one
       * @return Pointer to new projector setup
       **/
      Setup* setup(Id const& _setupId);

      /// Update projector matrix by current setup and return setup
      Setup* setup();

      /// Returns projector setup (const version)
      Setup const* setup() const;

      qreal aspectRatio() const;
      void setScreen(QScreen const* _screen);
      QScreen const* screen() const;
    
      qreal throwRatio() const;
      void setThrowRatio(qreal);

      Angle fov() const;
      void setFov(Angle _fov);

      QMatrix4x4& matrix();
      QMatrix4x4 const& matrix() const;
      
      QMatrix4x4 projectionMatrix() const;
      
      void setMatrix(QMatrix4x4 const&);

      QVector3D pos() const;

      friend bool operator==(Projector const&,Projector const&);

    private:
      QMatrix4x4 matrix_;
      QScreen const* screen_ = nullptr;
      Angle fov_;
      std::unique_ptr<Setup> setup_;
    };
  }

  using proj::Projector;
}

QDataStream& operator>>(QDataStream&, omni::proj::Projector&);
QDataStream& operator<<(QDataStream&, omni::proj::Projector const&);

#endif /* OMNI_PROJ_PROJECTOR_H_ */

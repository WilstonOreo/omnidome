#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    Projector::Projector() : 
      screen_(nullptr),
      fov_(45.0)
    {
    }

    Projector::Projector(Screen const* _screen,
                         Angle _fov) :
      screen_(_screen),
      fov_(_fov)
    {
    }

    Screen const* Projector::screen() const
    {
      return screen_;
    }

    void Projector::setScreen(Screen const* _screen)
    {
      screen_=_screen;
    }

    qreal Projector::aspectRatio() const
    {
      return qreal(screen_->width()) / screen_->height();
    }

    qreal Projector::throwRatio() const
    {
      return tan(fov_.radians() / 2.0) * aspectRatio();
    }

    void Projector::setThrowRatio(qreal _throwRatio)
    {
      fov_ = Angle::fromRad(2.0 * atan(_throwRatio  / aspectRatio()));
    }

    void Projector::setFov(Angle _fov)
    {
      fov_ = _fov;
      if (fov_.degrees() < 1.0) fov_ = 1.0;
      if (fov_.degrees() >= 180.0) fov_ = 180.0;
    }

    Angle Projector::fov() const
    {
      return fov_;
    }

    QMatrix4x4& Projector::matrix() 
    {
      return matrix_;
    }

    QMatrix4x4 const& Projector::matrix() const
    {
      return matrix_;
    }

    void Projector::setMatrix(QMatrix4x4 const& _matrix) 
    {
      matrix_=_matrix;
    }
  
    QVector3D Projector::pos() const
    {
      return matrix().column(3).toVector3D();
    }
  }
}

QDataStream& operator>>(QDataStream&, omni::proj::Projector&);
QDataStream& operator<<(QDataStream&, omni::proj::Projector const&);

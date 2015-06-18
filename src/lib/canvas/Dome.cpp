#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    Dome::Dome()
    {
      update();
    }

    Dome::~Dome()
    {
    }

    qreal Dome::radius() const
    {
      return sphere_.radius();
    }

    void Dome::setRadius(qreal _radius)
    {
      sphere_.setRadius(_radius);
      update();
    }

    qreal Dome::diameter() const
    {
      return radius() * 2.0;
    }

    void Dome::setDiameter(qreal _diameter)
    {
      setRadius(_diameter * 0.5);
    }
 
    Box Dome::bounds() const
    {
      return bounds_;
    }
      
    void Dome::update()
    {
      auto _r = radius();
      QVector3D _vr(_r,_r,_r);
      this->bounds_ = Box(-_vr,_vr); 
      sphere_.update();
    }

    void Dome::toStream(QDataStream& _stream) const
    {
      _stream << radius();
    }

    void Dome::fromStream(QDataStream& _stream)
    {
      qreal _radius;
      _stream >> _radius;
      setRadius(_radius);
      update();
    }
  }
}

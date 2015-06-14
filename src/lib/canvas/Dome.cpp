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
      return radius_;
    }

    void Dome::setRadius(qreal _radius)
    {
      radius_=_radius;
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
      _stream << radius_;
    }

    void Dome::fromStream(QDataStream& _stream)
    {
      _stream >> radius_; 
    }
  }
}

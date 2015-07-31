#include <omni/canvas/Dome.h>

namespace omni
{
  namespace canvas
  {
    Dome::Dome() : center_(0.0,0.0,0.0)
    {
      update();
    }

    Dome::~Dome()
    {
    }
      
    void Dome::draw() const
    {
      Envelope::draw();

      glPushMatrix();
      {
        glLoadIdentity();

        glTranslatef(center_.x(),center_.y(),center_.z());
        this->sphere_.draw();
      }
      glPopMatrix();
    }

    void Dome::drawAux() const
    {
    }

    qreal Dome::radius() const
    {
      return sphere_.radius();
    }

    void Dome::setRadius(qreal _radius)
    {
      sphere_.setRadius(_radius);
      needsUpdate_ = true;
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
      
    QVector3D Dome::center() const
    {
      return center_;
    }

    void Dome::setCenter(QVector3D const& _center)
    {
      center_=_center;
    }

    void Dome::update()
    {
      if (!this->needsUpdate_) return;

      auto _r = radius();
      QVector3D _vr(_r,_r,_r);
      this->bounds_ = Box(-_vr + center_,_vr + center_); 
      sphere_.update();
    }

    void Dome::toStream(QDataStream& _stream) const
    {
      _stream << radius();
      _stream << center();
    }

    void Dome::fromStream(QDataStream& _stream)
    {
      qreal _radius;
      _stream >> _radius;
      setRadius(_radius);
      QVector3D _center;
      _stream >> _center;
      setCenter(_center);
      update();
    }
  }
}

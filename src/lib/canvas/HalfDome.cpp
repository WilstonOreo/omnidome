#include <omni/canvas/HalfDome.h>

namespace omni
{
  namespace canvas
  {
    HalfDome::HalfDome(qreal _diameter)
    {
      this->sphere_.setBottom(0.0);
      setDiameter(_diameter);
    }

    HalfDome::~HalfDome()
    {
    }

    void HalfDome::draw() const
    {
      glPushMatrix();
      {
        glLoadIdentity();

        glTranslatef(center_.x(),center_.y(),center_.y());

        this->sphere_.draw();
      }
      glPopMatrix();

    }

    void HalfDome::drawAux() const
    {
    }
 
    EulerAngles& HalfDome::angles()
    {
      return angles_;
    }

    EulerAngles const& HalfDome::angles() const
    {
      return angles_;
    }
      
    QVector3D HalfDome::center() const
    {
      return center_;
    }

    void HalfDome::setCenter(QVector3D const& _center)
    {
      center_ = _center;
    }

    void HalfDome::update()
    {
      auto _r = radius();
      QVector3D _vr(_r,_r,_r);
      this->bounds_ = Box(-_vr + center_,_vr + center_); 
      sphere_.update();
    }
 
    void HalfDome::fromStream(QDataStream& _stream)
    {
      Dome::fromStream(_stream);
      _stream >> angles_ >> center_;
      update();
    }

    void HalfDome::toStream(QDataStream& _stream) const
    {
      Dome::toStream(_stream);
      _stream << angles_ << center_;
    }
  }
}

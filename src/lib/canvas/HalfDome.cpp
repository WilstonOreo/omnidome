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
        glTranslatef(center().x(),center().y(),center().z());
        glMultMatrixf(angles_.matrix().constData());
        this->sphere_.draw();
      }
      glPopMatrix();
    }
 
    EulerAngles& HalfDome::angles()
    {
      return angles_;
    }

    EulerAngles const& HalfDome::angles() const
    {
      return angles_;
    }
      
    void HalfDome::update()
    {
      auto _r = radius();
      QVector3D _vr(_r,_r,_r);
      this->bounds_ = Box(-_vr + center(),_vr + center()); 
      sphere_.update();
    }
 
    void HalfDome::fromStream(QDataStream& _stream)
    {
      Dome::fromStream(_stream);
      _stream >> angles_;
      update();
    }

    void HalfDome::toStream(QDataStream& _stream) const
    {
      Dome::toStream(_stream);
      _stream << angles_;
    }
  }
}

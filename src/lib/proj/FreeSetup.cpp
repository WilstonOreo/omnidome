#include <omni/proj/FreeSetup.h>

#include <QMatrix4x4>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    FreeSetup::FreeSetup() :
      pos_(0.0,0.0,0.0)
    {
    }

    void FreeSetup::toStream(QDataStream& _stream) const
    {
      AngleSetup::toStream(_stream);
      _stream << pos_;
    }

    void FreeSetup::fromStream(QDataStream& _stream)
    {
      AngleSetup::fromStream(_stream);
      _stream >> pos_;
    }

    void FreeSetup::setup(Projector& _proj)
    {
      AngleSetup::setup(_proj);
      _proj.matrix().translate(pos_);
    }

    QVector3D FreeSetup::pos() const
    {
      return pos_;
    }

    void FreeSetup::setPos(QVector3D const& _pos)
    {
      pos_=_pos;
    }

    void FreeSetup::setPos(float _x, float _y, float _z)
    {
      pos_ = QVector3D(_x,_y,_z);
    }
  }
}

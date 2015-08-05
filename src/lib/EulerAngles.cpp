#include <omni/EulerAngles.h>

#include <QDataStream>

namespace omni
{
  EulerAngles::EulerAngles() :
    yaw_(0), pitch_(0), roll_(0)
  {
  }

  EulerAngles::EulerAngles(Angle _yaw, Angle _pitch, Angle _roll) :
    yaw_(_yaw),
    pitch_(_pitch),
    roll_(_roll)
  {
  }

  Angle& EulerAngles::yaw()
  {
    return yaw_;
  }

  Angle const& EulerAngles::yaw() const
  {
    return yaw_;
  }

  void EulerAngles::setYaw(Angle _yaw)
  {
    yaw_ = _yaw;
  }

  Angle& EulerAngles::pitch()
  {
    return pitch_;
  }

  Angle const& EulerAngles::pitch() const
  {
    return pitch_;
  }

  void EulerAngles::setPitch(Angle _pitch)
  {
    pitch_ = _pitch;
  }

  Angle& EulerAngles::roll()
  {
    return roll_;
  }

  Angle const& EulerAngles::roll() const
  {
    return roll_;
  }

  void EulerAngles::setRoll(Angle _roll)
  {
    roll_ = _roll;
  }

  QMatrix4x4 EulerAngles::matrix(Angle _yaw, Angle _pitch, Angle _roll)
  {
    QMatrix4x4 _matrix;
    // Rotate ZYX
    _matrix.rotate(_yaw.degrees(),QVector3D(0.0,0.0,1.0));
    _matrix.rotate(_pitch.degrees(),QVector3D(0.0,1.0,0.0));
    _matrix.rotate(_roll.degrees(),QVector3D(1.0,0.0,0.0));
    return _matrix;
  }
 
  QMatrix4x4 EulerAngles::matrix() const
  {
    return matrix(yaw_,pitch_,roll_);
  }
}

QDataStream& operator>>(QDataStream& _stream, omni::EulerAngles& _angles)
{
  _stream >> _angles.yaw() >> _angles.pitch() >> _angles.roll();
  return _stream;
}

QDataStream& operator<<(QDataStream& _stream, omni::EulerAngles const& _angles)
{
  _stream << _angles.yaw() << _angles.pitch() << _angles.roll();
  return _stream;
}


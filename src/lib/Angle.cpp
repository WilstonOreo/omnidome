#include <omni/Angle.h>

#include <cmath>
#include <QDataStream>
#include <omni/util.h>

namespace omni
{
  Angle::Angle() : angle_(0.0)
  {
  }

  Angle::Angle(qreal _angle)
  {
    setDegrees(_angle);
  }

  /// Set angle by given degrees
  void Angle::setDegrees(qreal _angle)
  {
    angle_ = fmod(_angle,360.0);
  }

  /// Set angle by radians
  void Angle::setRadians(qreal _angle)
  {
    setDegrees(_angle * 180.0 / M_PI);
  }

  /// Return degrees
  qreal Angle::degrees() const
  {
    return angle_;
  }

  /// Returns radians
  qreal Angle::radians() const
  {
    return angle_ / 180.0 * M_PI;
  }

  /// Constructs from degrees
  Angle Angle::fromDeg(qreal _angle)
  {
    return Angle(_angle);
  }

  /// Constructs angle from radians
  Angle Angle::fromRad(qreal _angle)
  {
    return Angle(Angle::fromDeg(_angle).radians());
  }
    
  Angle const& Angle::operator+=(Angle const& _angle)
  {
    angle_ += _angle.degrees();
    return *this;
  }

  Angle const& Angle::operator-=(Angle const& _angle)
  {
    angle_ -= _angle.degrees();
    return *this;
  }
    
  bool operator==(Angle const& _lhs, Angle const& _rhs)
  {
    return OMNI_TEST_MEMBER_EQUAL(angle_);
  }
}

QDataStream& operator<<(QDataStream& _os, const omni::Angle& _angle)
{
  _os << _angle.degrees();
  return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::Angle& _angle)
{
  qreal _r;
  _is >> _r;
  _angle = omni::Angle(_r);
  return _is;
}

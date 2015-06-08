#ifndef OMNI_ANGLE_HPP_
#define OMNI_ANGLE_HPP_

#include <QtGlobal>

namespace omni
{
  /// Struct for representing an angle (stored in degrees internally)
  struct Angle
  {
    /// Default constructor
    Angle();

    /// Constructor from value
    Angle(qreal _angle);

    /// Set angle by given degrees
    void setDegrees(qreal _angle);

    /// Set angle by radians
    void setRadians(qreal _angle);

    /// Return degrees
    qreal degrees() const;

    /// Returns radians
    qreal radians() const;

    /// Constructs from degrees
    static Angle fromDeg(qreal _angle);

    /// Constructs angle from radians
    static Angle fromRad(qreal _angle);

    Angle const& operator+=(Angle const&);
    Angle const& operator-=(Angle const&);

  private:
    qreal angle_;
  };
}

QDataStream& operator<<(QDataStream& _os, const omni::Angle& _angle);

QDataStream& operator>>(QDataStream& _is, omni::Angle& _angle);



#endif /* OMNI_ANGLE_HPP_ */

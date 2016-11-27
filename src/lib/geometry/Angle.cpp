/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <omni/geometry/Angle.h>

#include <cmath>
#include <QDataStream>
#include <omni/util.h>

namespace omni {
  namespace geometry {
    Angle::Angle() : angle_(0.0)
    {}

    Angle::Angle(qreal _angle)
    {
      setDegrees(_angle);
    }

    /// Set angle by given degrees
    void Angle::setDegrees(qreal _angle)
    {
      angle_ = fmod(_angle, 360.0);
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
    Angle Angle::fromRad(qreal _rad)
    {
      Angle _angle;

      _angle.setRadians(_rad);
      return _angle;
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
}

QDataStream& operator<<(QDataStream& _os, const omni::geometry::Angle& _angle)
{
  _os << _angle.degrees();
  return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::geometry::Angle& _angle)
{
  qreal _r;

  _is >> _r;
  _angle = omni::Angle(_r);
  return _is;
}

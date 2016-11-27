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

#include <omni/geometry/EulerAngles.h>

#include <QDataStream>
#include <omni/util.h>

namespace omni {
  namespace geometry {
    EulerAngles::EulerAngles() :
      yaw_(0), pitch_(0), roll_(0)
    {}

    EulerAngles::EulerAngles(Angle _yaw, Angle _pitch, Angle _roll) :
      yaw_(_yaw),
      pitch_(_pitch),
      roll_(_roll)
    {}

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
      _matrix.rotate(_yaw.degrees(), QVector3D(0.0, 0.0, 1.0));
      _matrix.rotate(_pitch.degrees(), QVector3D(0.0, 1.0, 0.0));
      _matrix.rotate(_roll.degrees(), QVector3D(1.0, 0.0, 0.0));
      return _matrix;
    }

    QMatrix4x4 EulerAngles::matrix() const
    {
      return matrix(yaw_, pitch_, roll_);
    }

    bool operator==(EulerAngles const& _lhs, EulerAngles const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(yaw_) &&
        OMNI_TEST_MEMBER_EQUAL(pitch_) &&
        OMNI_TEST_MEMBER_EQUAL(roll_);
    }
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

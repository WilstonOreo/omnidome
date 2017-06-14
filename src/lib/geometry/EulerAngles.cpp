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
    EulerAngles::EulerAngles(QObject* parent) :
      QObject(parent)
    {}

    EulerAngles::EulerAngles(Angle _yaw, Angle _pitch, Angle _roll,
      QObject* parent) :
      QObject(parent),
      yaw_(_yaw),
      pitch_(_pitch),
      roll_(_roll)
    {}

    OMNI_PROPERTY_RW_IMPL(EulerAngles,Angle,yaw,setYaw)
    OMNI_PROPERTY_RW_IMPL(EulerAngles,Angle,pitch,setPitch)
    OMNI_PROPERTY_RW_IMPL(EulerAngles,Angle,roll,setRoll)

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
  }
}

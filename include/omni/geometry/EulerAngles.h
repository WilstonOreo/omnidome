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

#ifndef OMNI_GEOMETRY_EULERANGLES_H_
#define OMNI_GEOMETRY_EULERANGLES_H_

#include <QMatrix4x4>
#include <omni/geometry/Angle.h>
#include <omni/Serializer.h>
#include <omni/property.h>

namespace omni {
  namespace geometry {
    /// Mixin class for a zyx Euler Angle Rotation system
    class EulerAngles : public QObject, public Serializer<EulerAngles>
    {
      Q_OBJECT
      OMNI_PROPERTY_RW(Angle,yaw,setYaw)
      OMNI_PROPERTY_RW(Angle,pitch,setPitch)
      OMNI_PROPERTY_RW(Angle,roll,setRoll)
    public:
      /// Default constructor, all angles are zero
      EulerAngles(QObject* = nullptr);

      /// Constructor by values
      EulerAngles(Angle _yaw,
                  Angle _pitch,
                  Angle _roll,
                  QObject* _parent = nullptr);

      /// Public Static function to get rotation matrix conveniently
      static QMatrix4x4 matrix(Angle _yaw,
                               Angle _pitch,
                               Angle _roll);

      /// Calculate the rotation matrix
      QMatrix4x4  matrix() const;
    signals:
        void yawChanged();
        void pitchChanged();
        void rollChanged();
    };
  }
  using geometry::EulerAngles;
}


#endif /* OMNI_GEOMETRY_EULERANGLES_H_ */

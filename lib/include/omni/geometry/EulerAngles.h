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

namespace omni {
  namespace geometry {
    /// Mixin class for a zyx Euler Angle Rotation system
    struct OMNI_EXPORT EulerAngles
    {
      /// Default constructor, all angles are zero
      EulerAngles();

      /// Constructor by values
      EulerAngles(Angle _yaw,
                  Angle _pitch,
                  Angle _roll);

      /// Reference to yaw angle (Angle in Z direction)
      Angle           & yaw();

      /// Reference to yaw angle (Angle in Z direction, const version)
      Angle const     & yaw() const;

      /// Set yaw angle (Z direction angle) to new value
      void              setYaw(Angle _yaw);

      /// Reference to pitch angle (Angle in Y direction)
      Angle           & pitch();

      /// Reference to pitch angle (Angle in Y direction, const version)
      Angle const     & pitch() const;

      /// Set pitch angle (Y direction angle) to new value
      void              setPitch(Angle _pitch);

      /// Reference to roll angle (Angle in X direction)
      Angle           & roll();

      /// Reference to roll angle (Angle in X direction)
      Angle const     & roll() const;

      /// Set roll angle (X direction angle) to new value
      void              setRoll(Angle _roll);

      /// Public Static function to get rotation matrix conveniently
      static QMatrix4x4 matrix(Angle _yaw,
                               Angle _pitch,
                               Angle _roll);

      /// Calculate the rotation matrix
      QMatrix4x4  matrix() const;

      /// Test for equality
      friend bool operator==(EulerAngles const&,
                             EulerAngles const&);

      private:
        /// Yaw Angle (Z direction)
        Angle yaw_;

        /// Pitch Angle (Y direction)
        Angle pitch_;

        /// Roll Angle (X direction)
        Angle roll_;
    };
  }
  using geometry::EulerAngles;
}

QDataStream& operator>>(QDataStream&,
                        omni::geometry::EulerAngles&);
QDataStream& operator<<(QDataStream&,
                        omni::geometry::EulerAngles const&);


#endif /* OMNI_GEOMETRY_EULERANGLES_H_ */

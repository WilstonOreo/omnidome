/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_GEOMETRY_ANGLE_H_
#define OMNI_GEOMETRY_ANGLE_H_

#include <omni/global.h>

namespace omni {
  namespace geometry {
    /// Struct for representing an angle (stored in degrees internally)
    struct OMNI_EXPORT Angle
    {
      /// Default constructor
      Angle();

      /// Constructor from value
      Angle(qreal _angle);

      /// Set angle by given degrees
      void         setDegrees(qreal _angle);

      /// Set angle by radians
      void         setRadians(qreal _angle);

      /// Return degrees
      qreal        degrees() const;

      /// Returns radians
      qreal        radians() const;

      /// Constructs from degrees
      static Angle fromDeg(qreal _angle);

      /// Constructs angle from radians
      static Angle fromRad(qreal _angle);

      /// Increment angle by another
      Angle const& operator+=(Angle const&);

      /// Decrement angle by another
      Angle const& operator-=(Angle const&);

      /// Test for equality
      friend bool  operator==(Angle const&,
                              Angle const&);

      private:
        qreal angle_;
    };
  }
  using geometry::Angle;
}

QDataStream& operator<<(QDataStream& _os,
                        const omni::geometry::Angle& _angle);
QDataStream& operator>>(QDataStream& _is,
                        omni::geometry::Angle& _angle);

#endif /* OMNI_GEOMETRY_ANGLE_H_ */

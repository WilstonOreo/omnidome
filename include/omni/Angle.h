/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#ifndef OMNI_ANGLE_H_
#define OMNI_ANGLE_H_

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
      
    friend bool operator==(Angle const&,Angle const&);

  private:
    qreal angle_;
  };
}

QDataStream& operator<<(QDataStream& _os, const omni::Angle& _angle);

QDataStream& operator>>(QDataStream& _is, omni::Angle& _angle);



#endif /* OMNI_ANGLE_H_ */

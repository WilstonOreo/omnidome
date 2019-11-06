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

#ifndef OMNI_GEOMETRY_POLARVEC_H_
#define OMNI_GEOMETRY_POLARVEC_H_

#include <QVector3D>
#include <QVector4D>
#include <QDataStream>
#include <omni/geometry/Angle.h>
#include <omni/serialization/Interface.h>

namespace omni {
  namespace geometry {
    /** @brief polar 3D vector defined by th two angles phi, theta and a radial
       distance
     **/
    struct OMNI_EXPORT PolarVec
    {
      public:
        /// default constructor
        PolarVec();

        /// Construct from QVector3D
        PolarVec(const QVector3D&);

        /// Construct from QVector4D
        PolarVec(const QVector4D&);

        /// Construct from longitude angle, latitude angle and radius
        PolarVec(Angle _longitude,
                 Angle _latitude,
                 qreal _radius);

        /// Assign from QVector3D
        const PolarVec& operator=(const QVector3D&);

        /// QVector3D cast operator
        operator QVector3D() const;

        /// QVector4D cast operator
        operator QVector4D() const;

        /// Increase by other PolarVec
        const PolarVec& operator+=(const PolarVec&);

        /// Return x value (by converting to QVector3D internally)
        qreal     x() const;

        /// Return y value (by converting to QVector3D internally)
        qreal     y() const;

        /// Return z value (by converting to QVector3D internally)
        qreal     z() const;

        /// Return converted QVector3D
        QVector3D vec() const;

        /// Returns normalized QVector3D
        QVector3D normalized() const;

        /// Return longitudinal angle
        Angle     longitude() const;

        /// Set longitudinal angle
        void setLongitude(Angle);

        /// Return latitudinal angle
        Angle latitude() const;

        /// Set latitudinal angle
        void setLatitude(Angle);

        /// Return radius
        qreal radius() const;

        /// Set radius
        void setRadius(qreal);

        /// Write transformation to stream
        void        toStream(QDataStream&) const;

        /// Read transformation from stream
        void        fromStream(QDataStream&);

        /// Test for equality
        friend bool operator==(PolarVec const&,
                               PolarVec const&);

      private:
        Angle longitude_, latitude_;
        qreal radius_;
    };
  }

  using geometry::PolarVec;
}

OMNI_DECL_STREAM_OPERATORS(omni::geometry::PolarVec)

#endif /* OMNI_GEOMETRY_POLARVEC_H_ */

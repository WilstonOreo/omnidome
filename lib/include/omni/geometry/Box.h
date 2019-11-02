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

#ifndef OMNI_GEOMETRY_BOX_H_
#define OMNI_GEOMETRY_BOX_H_

#include <QVector3D>
#include <omni/global.h>

namespace omni {
  namespace geometry {
    /// A box defines a 3D bounding box with a min and max point
    class OMNI_EXPORT Box {
      public:
        /// Default constructor
        Box();

        /// Constructor by min and max vector
        Box(QVector3D const& _min,
            QVector3D const& _max);

        /// Returns the size of the box (= max - min)
        QVector3D size() const;

        /// Returns radius of the box (= half length of size vector)
        qreal     radius() const;

        /// Returns the minimum point
        QVector3D min() const;

        /// Returns the maximum point
        QVector3D max() const;

        /// Set minimum point of the box
        void      setMin(QVector3D const&);

        /// Set maximum point of the box
        void      setMax(QVector3D const&);

        /// Set minimum and maximum together
        void      setMinMax(QVector3D const&,
                            QVector3D const&);

        /// Extend dimenstions of box by point
        void      extend(QVector3D const&);

        /// Return the center point of the box
        QVector3D center() const;

      private:
        /// Check if min < max and swap if necessary
        void validate();

        QVector3D min_, max_;
    };
  }
  using geometry::Box;
}


#endif /* OMNI_GEOMETRY_BOX_H_ */

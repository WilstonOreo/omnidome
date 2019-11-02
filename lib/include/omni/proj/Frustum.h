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

#ifndef OMNI_PROJ_FRUSTUM_H_
#define OMNI_PROJ_FRUSTUM_H_

#include <QVector3D>
#include <QRectF>
#include <omni/global.h>

namespace omni {
  namespace proj {
    class Projector;

    /// Frustum helper struct constructed from a projector
    class OMNI_EXPORT Frustum {
      public:
        /// Frustum must be constructed from a projector
        Frustum(Projector const&);

        /// Eye position of frustum
        QVector3D eye() const;

        /// Vector of top left corner
        QVector3D topLeft() const;

        /// Vector of top left corner, transformed by matrix
        QVector3D topLeft(QMatrix4x4 const&) const;

        /// Vector of top right corner
        QVector3D topRight() const;

        /// Vector of top right corner, transformed by matrix
        QVector3D topRight(QMatrix4x4 const&) const;

        /// Vector of bottom left corner
        QVector3D bottomLeft() const;

        /// Vector of bottom left corner, transformed by matrix
        QVector3D bottomLeft(QMatrix4x4 const&) const;

        /// Vector of bottom right corner
        QVector3D bottomRight() const;

        /// Vector of bottom right corner, transformed by matrix
        QVector3D bottomRight(QMatrix4x4 const&) const;

        /// Return frustum rectangle
        QRectF    rect() const;

        /// Return frustum rectangle, multiplied by near and far values
        QRectF    rect(float _near,
                       float _far) const;

      private:
        Projector const& proj_;
    };
  }
}

#endif /* OMNI_PROJ_FRUSTUM_H_ */

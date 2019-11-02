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

#ifndef OMNI_GEOMETRY_VERTEX2D_H_
#define OMNI_GEOMETRY_VERTEX2D_H_

#include <QVector2D>
#include <omni/global.h>

namespace omni {
  namespace geometry {
    /// A Warp vertex is a geometry entity with a position, normal and texture
    // coordinates
    struct OMNI_EXPORT Vertex2D {
      /// Default constructor
      Vertex2D();

      /// Construct with position and texture coordinates
      Vertex2D(
        const QVector2D& _pos,
        const QVector2D& _texCoord);

      /// Set new position
      void             setPos(QVector2D const& _pos);

      /// Return reference to position
      QVector2D      & pos();

      /// Return const reference to position
      QVector2D const& pos() const;

      /// Set new texture coordinates
      void             setTexCoord(QVector2D const& _texCoord);

      /// Return reference to texture coordinates
      QVector2D      & texCoord();

      /// Return const reference to texture coordinates
      QVector2D const& texCoord() const;

      inline static constexpr size_t texCoordOffset()
      {
        return 0;
      }

      inline static constexpr size_t posOffset()
      {
        return sizeof(QVector2D);
      }

      private:
        QVector2D texCoord_;
        QVector2D pos_;
    };
  }
  using geometry::Vertex2D;
}

#endif /* OMNI_GEOMETRY_VERTEX2D_H_ */

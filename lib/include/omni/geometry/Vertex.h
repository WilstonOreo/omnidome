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

#ifndef OMNI_GEOMETRY_VERTEX_H_
#define OMNI_GEOMETRY_VERTEX_H_

#include <QVector2D>
#include <QVector3D>
#include <omni/global.h>

namespace omni {
  namespace geometry {
    /// A vertex is a geometry entity with a position, normal and texture
    // coordinates
    struct OMNI_EXPORT Vertex
    {
      /// Default constructor
      Vertex();

      /// Construct from given position, normal and optional texture coordinates
      Vertex(
        const QVector3D& _pos,
        const QVector3D& _normal,
        const QVector3D& _texCoord = QVector3D());

      /// Set new position
      void             setPos(QVector3D const& _pos);

      /// Return reference to position
      QVector3D      & pos();

      /// Return const reference to position
      QVector3D const& pos() const;

      /// Sets new normal (is not normalized!)
      void             setNormal(QVector3D const& _normal);

      /// Return reference to normal
      QVector3D      & normal();

      /// Return const reference to normal
      QVector3D const& normal() const;

      /// Set new texture coordinates
      void             setTexCoord(QVector3D const& _texCoord);


      /// Return reference to texture coordinates
      QVector3D      & texCoord();

      /// Return const reference to texture coordinates
      QVector3D const& texCoord() const;

      inline static constexpr size_t texCoordOffset()
      {
        return 0;
      }

      inline static constexpr size_t normalOffset()
      {
        return sizeof(QVector3D);
      }

      inline static constexpr size_t posOffset()
      {
        return sizeof(QVector3D) + sizeof(QVector3D);
      }

      private:
        QVector3D texCoord_;
        QVector3D normal_;
        QVector3D pos_;
    };
  }
  using geometry::Vertex;
}

#endif /* OMNI_GEOMETRY_VERTEX_H_ */

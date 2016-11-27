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

#include <omni/geometry/Vertex2D.h>

namespace omni {
  namespace geometry {
    Vertex2D::Vertex2D() {}

    Vertex2D::Vertex2D(
      const QVector2D& _pos,
      const QVector2D& _texCoord) :
      texCoord_(_texCoord),
      pos_(_pos)
    {}

    void Vertex2D::setPos(QVector2D const& _pos) {
      pos_ = _pos;
    }

    QVector2D& Vertex2D::pos() {
      return pos_;
    }

    QVector2D const& Vertex2D::pos() const {
      return pos_;
    }

    void Vertex2D::setTexCoord(QVector2D const& _texCoord) {
      texCoord_ = _texCoord;
    }

    QVector2D& Vertex2D::texCoord() {
      return texCoord_;
    }

    QVector2D const& Vertex2D::texCoord() const {
      return texCoord_;
    }
  }
}

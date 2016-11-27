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

#include <omni/geometry/Vertex.h>

namespace omni {
  namespace geometry {
    Vertex::Vertex()
    {}

    Vertex::Vertex(
      const QVector3D& _pos,
      const QVector3D& _normal,
      const QVector3D& _texCoord) :
      texCoord_(_texCoord),
      normal_(_normal),
      pos_(_pos)
    {}

    void Vertex::setPos(QVector3D const& _pos)
    {
      pos_ = _pos;
    }

    QVector3D& Vertex::pos()
    {
      return pos_;
    }

    QVector3D const& Vertex::pos() const
    {
      return pos_;
    }

    void Vertex::setNormal(QVector3D const& _normal)
    {
      normal_ = _normal;
    }

    QVector3D& Vertex::normal()
    {
      return normal_;
    }

    QVector3D const& Vertex::normal() const
    {
      return normal_;
    }

    void Vertex::setTexCoord(QVector3D const& _texCoord)
    {
      texCoord_ = _texCoord;
    }

    QVector3D& Vertex::texCoord()
    {
      return texCoord_;
    }

    QVector3D const& Vertex::texCoord() const
    {
      return texCoord_;
    }
  }
}

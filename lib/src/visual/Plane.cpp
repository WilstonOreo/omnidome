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

#include <omni/visual/Plane.h>

namespace omni {
  namespace visual {
    Plane::Plane()
    {}

    Plane::~Plane() {}

    void Plane::draw() const {
      if (!vbo_) return;
      vbo_->bindAndDraw(vbo_->numIndices(), GL_QUADS);
    }

    void Plane::update(texcoord_transform_functor_type _texTrans) {
      size_t _resX        = horizontal_ + 1;
      size_t _resY        = vertical_ + 1;
      size_t _numVertices = _resX * _resY;
      VertexVBO::vertex_buffer_type _vertices(_numVertices);
      VertexVBO::index_buffer_type  _indices(4 * _numVertices);

      auto _vertexIt = _vertices.begin();
      auto _indexIt  = _indices.begin();

      for (size_t x = 0; x < _resX; ++x) {
        for (size_t y = 0; y < _resY; ++y) {
          // index
          if (((x + 1) < _resX) && ((y + 1) < _resY)) {
            *(_indexIt++) = (x + 0) * _resY + (y + 0);
            *(_indexIt++) = (x + 1) * _resY + (y + 0);
            *(_indexIt++) = (x + 1) * _resY + (y + 1);
            *(_indexIt++) = (x + 0) * _resY + (y + 1);
          }

          // texCoords
          QVector2D _texCoord(x / (float)(_resX - 1),
                              1.0 - y / (float)(_resY - 1));
          _texTrans(_texCoord, x, y);

          _vertexIt->setTexCoord(_texCoord);
          _vertexIt->setPos(QVector3D(_texCoord.x() - 0.5, _texCoord.y() - 0.5,
                                      0.0));
          _vertexIt->setNormal(QVector3D(0.0, 0.0, 1.0));
          ++_vertexIt;
        }
      }

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        vbo_.reset(new VertexVBO(_vertices, _indices));
      });
    }

    void Plane::update() {
      update([](QVector2D&, size_t, size_t) {});
    }

    size_t Plane::horizontal() const {
      return horizontal_;
    }

    void Plane::setHorizontal(size_t _horizontal) {
      if (_horizontal < 2) _horizontal = 2;

      if (_horizontal > 0xFF) _horizontal = 0xFF;
      horizontal_ = _horizontal;
    }

    size_t Plane::vertical() const {
      return vertical_;
    }

    void Plane::setVertical(size_t _vertical) {
      if (_vertical < 2) _vertical = 2;

      if (_vertical > 0xFF) _vertical = 0xFF;
      vertical_ = _vertical;
    }

    void Plane::resize(size_t _horizontal, size_t _vertical) {
      setHorizontal(_horizontal);
      setVertical(_vertical);
    }
  }
}

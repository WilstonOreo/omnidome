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
#include <omni/visual/Box.h>

#include <array>

namespace omni {
  namespace visual {
    Box::Box() {}

    Box::~Box() {}

    void Box::draw() const {
      vbo_.bindAndDraw(vbo_.numIndices(), GL_QUADS);
    }

    void Box::update() {
      size_t _resX        = horizontal_ + 1;
      size_t _resY        = vertical_ + 1;
      size_t _numVertices = _resX * _resY;
      VertexVBO::vertex_buffer_type _vertices(6 * _numVertices);
      VertexVBO::index_buffer_type  _indices(6 * 4 * _numVertices);

      auto _vertexIt = _vertices.begin();
      auto _indexIt  = _indices.begin();

      std::array<QVector3D, 6> _normals = {
        QVector3D(0.0,    1.0, 0.0),
        QVector3D(0.0,   -1.0, 0.0),
        QVector3D(1.0,    0.0, 0.0),
        QVector3D(-1.0,   0.0, 0.0),
        QVector3D(0.0,    0.0, 1.0),
        QVector3D(0.0,    0.0, -1.0)
      };

      for (size_t i = 0; i < 6; ++i) {
        auto& _normal = _normals[i];

        for (size_t x = 0; x < _resX; ++x) {
          for (size_t y = 0; y < _resY; ++y) {
            // index
            if (((x + 1) < _resX) && ((y + 1) < _resY)) {
              *(_indexIt++) = i * _numVertices  + (x + 0) * _resY + (y + 1);
              *(_indexIt++) = i * _numVertices +  (x + 1) * _resY + (y + 1);
              *(_indexIt++) = i * _numVertices + (x + 1) * _resY + (y + 0);
              *(_indexIt++) = i * _numVertices + (x + 0) * _resY + (y + 0);
            }

            // texCoords
            QVector2D _pos(x / float(_resX - 1), y / float(_resY - 1));
            QVector2D _texCoord((i + 1.0 - _pos.x()) / 6.0, _pos.y());
            _pos -= QVector2D(0.5, 0.5);

            switch (i)
            {
            case 0:
              _vertexIt->setPos(QVector3D(_pos.x(), _normal.y() * 0.5, _pos.y()));
              break;

            case 1:
              _vertexIt->setPos(QVector3D(-_pos.x(), _normal.y() * 0.5,
                                          _pos.y()));
              break;

            case 2:
              _vertexIt->setPos(QVector3D(_normal.x() * 0.5, -_pos.x(),
                                          _pos.y()));
              break;

            case 3:
              _vertexIt->setPos(QVector3D(_normal.x() * 0.5, _pos.x(), _pos.y()));
              break;

            case 4:
              _vertexIt->setPos(QVector3D(-_pos.x(), _pos.y(),
                                          _normal.z() * 0.5));
              break;

            case 5:
              _vertexIt->setPos(QVector3D(-_pos.x(), -_pos.y(),
                                          _normal.z() * 0.5));
              break;
            }

            _vertexIt->setTexCoord(_texCoord);

            _vertexIt->setNormal(_normals[i]);
            ++_vertexIt;
          }
        }
      }

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        vbo_.buffer(_vertices, _indices);

      });
    }
  }
}

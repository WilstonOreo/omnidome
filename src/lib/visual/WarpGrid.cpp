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

#include <omni/visual/WarpGrid.h>

#include <iostream>
#include <array>
#include <QPointF>
#include <QRectF>
#include <QColor>
#include <omni/visual/util.h>

namespace omni {
  namespace visual {
    WarpGrid::WarpGrid(omni::WarpGrid const& _warpGrid) :
      warpGrid_(_warpGrid)
    {}

    WarpGrid::~WarpGrid()
    {}

    void WarpGrid::draw() const
    {
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        _.glBindBuffer(GL_ARRAY_BUFFER,         vertexVbo_.id());
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo_.id());

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2D),
                          (void *)Vertex2D::texCoordOffset());
        glVertexPointer(2, GL_FLOAT, sizeof(Vertex2D),
                        (void *)Vertex2D::posOffset());

        _.glDrawElements(GL_QUADS, indices_.size() - 4,
                         GL_UNSIGNED_INT, 0);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        _.glBindBuffer(GL_ARRAY_BUFFER,         0);
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      });
    }

    void WarpGrid::drawLines()
    {
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glLineWidth(2.0);
        _.glBindBuffer(GL_ARRAY_BUFFER,         gridVertexVbo_.id());
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIndexVbo_.id());

        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(2, GL_FLOAT, sizeof(QVector2D), 0);

        _.glDrawElements(GL_LINES, gridIndices_.size() - 4,
                         GL_UNSIGNED_INT, 0);

        glDisableClientState(GL_VERTEX_ARRAY);

        _.glBindBuffer(GL_ARRAY_BUFFER,         0);
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      });
    }

    void WarpGrid::drawHandles(QColor const& _color, QRectF const& _rect)
    {
      if (!circle_) return;

      const float _radius = 0.1 / sqrt(
        warpGrid_.horizontal() * warpGrid_.vertical());
      float _rX = _radius * _rect.width();
      float _rY = _radius * _rect.height();

      withCurrentContext([&](QOpenGLFunctions& _)
      {
        for (auto& _point : warpGrid_.points())
        {
          glColor4f(1.0, 1.0, 1.0, 0.5);
          circle_->drawLine(_point.pos(), _rX, _rY);

          if (_point.selected())
          {
            glColor4f(_color.redF(), _color.greenF(), _color.blueF(),
                      0.5);
            circle_->drawFill(_point.pos(), _rX, _rY);
          }
        }
      });
    }

    int WarpGrid::subdivisions() const {
      return subdivisions_;
    }

    void WarpGrid::setSubdivisions(int _subdivisions) {
      subdivisions_ = _subdivisions;
      update();
    }

    void WarpGrid::update()
    {
      if (!warpGrid_.hasChanged()) {
        return;
      }

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        if (!circle_) {
          circle_.reset(new Circle());
        }


        vertexVbo_.gen();
        indexVbo_.gen();
        gridVertexVbo_.gen();
        gridIndexVbo_.gen();

        size_t _resX        = subdivisions() * (warpGrid_.horizontal() - 1) + 1;
        size_t _resY        = subdivisions() * (warpGrid_.vertical() - 1) + 1;
        size_t _numVertices = _resX * _resY;
        bool _resized = vertices_.size() != _numVertices;

        if (_resized) {
          vertices_.clear();
          vertices_.resize(_numVertices);
          indices_.clear();
          indices_.resize(4 * _numVertices);

          gridVertices_.clear();
          size_t _num = (2 * subdivisions()) * warpGrid_.horizontal() * warpGrid_.vertical();
          gridVertices_.resize(_num);
          gridIndices_.clear();
          gridIndices_.resize(_num * 2);
        }

        auto _vertexIt = vertices_.begin();
        auto _indexIt = indices_.begin();

        for (size_t x = 0; x < _resX; ++x) {
          for (size_t y = 0; y < _resY; ++y) {
            // index
            if (_resized) {
              if (((x + 1) < _resX) && ((y + 1) < _resY)) {
                *(_indexIt++) = (x + 0) * _resY + (y + 0);
                *(_indexIt++) = (x + 1) * _resY + (y + 0);
                *(_indexIt++) = (x + 1) * _resY + (y + 1);
                *(_indexIt++) = (x + 0) * _resY + (y + 1);
              }

              // texCoords
              float tx = x / (float)(_resX - 1);
              float ty = y / (float)(_resY - 1);
              _vertexIt->setTexCoord(QVector2D(tx, 1.0 - ty));
            }

            // transform coordinates to [0..numControls]
            float u = x * (warpGrid_.horizontal() - 1) /
                      (float)(_resX - 1);
            float v = y * (warpGrid_.vertical() - 1) /
                      (float)(_resY - 1);

            // determine col and row
            int _col = (int)(u);
            int _row = (int)(v);

            // normalize coordinates to [0..1]
            u -= _col;
            v -= _row;

            _vertexIt->setPos(warpGrid_.getWarpPointPos(_col, _row, u, v));
            ++_vertexIt;
          }
        }

        /// Generate grid
        auto _gridVertexIt = gridVertices_.begin();
        auto _gridIndexIt = gridIndices_.begin();

        // Generate horizontal lines
        size_t i = 0;

        for (size_t y = 0; y < _resY; y += subdivisions()) {
          for (size_t x = 0; x < _resX; ++x) {
            *(_gridVertexIt++) = vertices_[x * _resY + y].pos();

            if (x < _resX - 1) {
              *(_gridIndexIt++) = i;
              *(_gridIndexIt++) = i + 1;
            }
            ++i;
          }
        }

        // Generate vertical lines
        for (size_t x = 0; x < _resX; x += subdivisions()) {
          for (size_t y = 0; y < _resY; ++y) {
            *(_gridVertexIt++) = vertices_[x * _resY + y].pos();

            if (y < _resY - 1) {
              *(_gridIndexIt++) = i;
              *(_gridIndexIt++) = i + 1;
            }
            ++i;
          }
        }

        vertexVbo_.bufferStaticArray(vertices_);
        indexVbo_.bufferStaticElementArray(indices_);

        gridVertexVbo_.bufferStaticArray(gridVertices_);
        gridIndexVbo_.bufferStaticElementArray(gridIndices_);
      });
    }
  }
}

/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
        {
        }

        WarpGrid::~WarpGrid()
        {}

        void WarpGrid::draw() const
        {
            with_current_context([this](QOpenGLFunctions& _)
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
            with_current_context([this](QOpenGLFunctions& _)
            {
                _.glEnable(GL_BLEND);
                glColor4f(1.0, 1.0, 1.0, 0.2);
                glLineWidth(2.0);
                _.glBindBuffer(GL_ARRAY_BUFFER,         gridVertexVbo_.id());
                _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIndexVbo_.id());

                glEnableClientState(GL_VERTEX_ARRAY);

                glVertexPointer(2, GL_FLOAT, sizeof(QVector2D),0);

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

            const float _radius           = 0.1 / sqrt(
                warpGrid_.horizontal() * warpGrid_.vertical());
            float _rX = _radius * _rect.width();
            float _rY = _radius * _rect.height();

            visual::with_current_context([&](QOpenGLFunctions& _)
            {
                _.glEnable(GL_BLEND);

                for (auto& _point : warpGrid_.points())
                {
                    glColor4f(1.0, 1.0, 1.0, 0.5);
                    circle_->drawLine(_point.pos(),_rX,_rY);

                    if (_point.selected())
                    {
                        glColor4f(_color.redF(), _color.greenF(), _color.blueF(),
                                  0.5);
                        circle_->drawFill(_point.pos(),_rX,_rY);
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

           // from http://www.paulinternet.nl/?page=bicubic : fast catmull-rom calculation
           QVector2D WarpGrid::cubicInterpolate(
               const std::array<QVector2D,4>& knots,
               float t ) const
           {
                return knots[1] + 0.5f * t*(knots[2] - knots[0] +
                        t*(2.0f*knots[0] - 5.0f*knots[1] +
                        4.0f*knots[2] - knots[3] +
                        t*(3.0f*(knots[1] - knots[2]) +
                        knots[3] - knots[0])));
           }
/*
           void WarpBilinear::setNumControlX(int n)
           {
                // there should be a minimum of 2 control points
                n = math<int>::max(2, n);

                // create a list of new points
                std::vector<Vec2f> temp(n * mControlsY);

                // perform spline fitting
                for(int row=0;row<mControlsY;++row) {
                        std::vector<Vec2f> points;
                        if(mIsLinear) {
                                // construct piece-wise linear spline
                                for(int col=0;col<mControlsX;++col) {
                                        points.push_back( getPoint(col, row) );
                                }

                                BSpline2f s( points, 1, false, true );

                                // calculate position of new control points
                                float length = s.getLength(0.0f, 1.0f);
                                float step = 1.0f / (n-1);
                                for(int col=0;col<n;++col) {
                                        temp[(col * mControlsY) + row] =
                                           s.getPosition( s.getTime( length *
                                           col * step ) );
                                }
                        }
                        else {
                                // construct piece-wise catmull-rom spline
                                for(int col=0;col<mControlsX;++col) {
                                        Vec2f p0 = getPoint(col-1, row);
                                        Vec2f p1 = getPoint(col, row);
                                        Vec2f p2 = getPoint(col+1, row);
                                        Vec2f p3 = getPoint(col+2, row);

                                        // control points according to an
                                           optimized Catmull-Rom implementation
                                        Vec2f b1 = p1 + (p2 - p0) / 6.0f;
                                        Vec2f b2 = p2 - (p3 - p1) / 6.0f;

                                        points.push_back(p1);

                                        if(col < (mControlsX-1)) {
                                                points.push_back(b1);
                                                points.push_back(b2);
                                        }
                                }

                                BSpline2f s( points, 3, false, true );

                                // calculate position of new control points
                                float length = s.getLength(0.0f, 1.0f);
                                float step = 1.0f / (n-1);
                                for(int col=0;col<n;++col) {
                                        temp[(col * mControlsY) + row] =
                                           s.getPosition( s.getTime( length *
                                           col * step ) );
                                }
                        }
                }

                // copy new control points
                mPoints = temp;
                mControlsX = n;

                mIsDirty = true;
           }

           void WarpBilinear::setNumControlY(int n)
           {
                // there should be a minimum of 2 control points
                n = math<int>::max(2, n);

                // create a list of new points
                std::vector<Vec2f> temp(mControlsX * n);

                // perform spline fitting
                for(int col=0;col<mControlsX;++col) {
                        std::vector<Vec2f> points;
                        if(mIsLinear) {
                                // construct piece-wise linear spline
                                for(int row=0;row<mControlsY;++row)
                                        points.push_back( getPoint(col, row) );

                                BSpline2f s( points, 1, false, true );

                                // calculate position of new control points
                                float length = s.getLength(0.0f, 1.0f);
                                float step = 1.0f / (n-1);
                                for(int row=0;row<n;++row) {
                                        temp[(col * n) + row] = s.getPosition(
                                           s.getTime( length * row * step ) );
                                }
                        }
                        else {
                                // construct piece-wise catmull-rom spline
                                for(int row=0;row<mControlsY;++row) {
                                        Vec2f p0 = getPoint(col, row-1);
                                        Vec2f p1 = getPoint(col, row);
                                        Vec2f p2 = getPoint(col, row+1);
                                        Vec2f p3 = getPoint(col, row+2);

                                        // control points according to an
                                           optimized Catmull-Rom implementation
                                        Vec2f b1 = p1 + (p2 - p0) / 6.0f;
                                        Vec2f b2 = p2 - (p3 - p1) / 6.0f;

                                        points.push_back(p1);

                                        if(row < (mControlsY-1)) {
                                                points.push_back(b1);
                                                points.push_back(b2);
                                        }
                                }

                                BSpline2f s( points, 3, false, true );

                                // calculate position of new control points
                                float length = s.getLength(0.0f, 1.0f);
                                float step = 1.0f / (n-1);
                                for(int row=0;row<n;++row) {
                                        temp[(col * n) + row] = s.getPosition(
                                           s.getTime( length * row * step ) );
                                }
                        }
                }

                // copy new control points
                mPoints = temp;
                mControlsY = n;

                mIsDirty = true;
           }
         */
        float WarpGrid::lerp(float v0, float v1, float t) const {
            return v0 + t * (v1 - v0);
        }

        void WarpGrid::update()
        {
            if(!circle_) {
                circle_.reset(new Circle());
            }
            circle_->update();
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
                size_t _num = (2*subdivisions()) * warpGrid_.horizontal() * warpGrid_.vertical();
                gridVertices_.resize(_num);
                gridIndices_.clear();
                gridIndices_.resize(_num*2);
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
                        float tx = lerp(0.0f, 1.0f, x / (float)(_resX - 1));
                        float ty = lerp(0.0f, 1.0f, y / (float)(_resY - 1));
                        _vertexIt->setTexCoord(QVector2D(tx, 1.0 - ty));
                    }

                    // transform coordinates to [0..numControls]
                    float u = x * (warpGrid_.horizontal() - 1) /
                            (float)(_resX - 1);
                    float v = y * (warpGrid_.vertical() - 1) /
                            (float)(_resY - 1);

                    // determine col and row
                    int col = (int)(u);
                    int row = (int)(v);

                    // normalize coordinates to [0..1]
                    u -= col;
                    v -= row;

                    QVector2D p;

                    // perform linear interpolation
                    /*
                    auto _p00   = warpGrid_.getWarpPointPos(col + 0, row + 0);
                    auto _p10   = warpGrid_.getWarpPointPos(col + 1, row + 0);
                    auto _p01   = warpGrid_.getWarpPointPos(col + 0, row + 1);
                    auto _p11   = warpGrid_.getWarpPointPos(col + 1, row + 1);
                    QVector2D p1((1.0f - u) * _p00 + u * _p10);
                    QVector2D p2((1.0f - u) * _p01 + u * _p11);

                    p = QVector2D((1.0f - v) * p1 + v * p2);
                    */
                    // perform bicubic interpolation


                    array4_type _rows, _cols;

                    for(int i=-1;i<3;++i) {
                        for(int j=-1;j<3;++j) {
                            auto _point = warpGrid_.getWarpPointPos(col + i, row + j);
                            _cols[j+1] = _point;
                        }
                        _rows[i+1] = cubicInterpolate( _cols, v );
                    }
                    p = cubicInterpolate( _rows, u );


                    _vertexIt->setPos(p);
                    ++_vertexIt;
                }
            }

            /// Generate grid
            auto _gridVertexIt = gridVertices_.begin();
            auto _gridIndexIt = gridIndices_.begin();

            size_t i = 0;
            for (size_t y = 0; y < _resY; y += subdivisions()) {
                for (size_t x = 0; x < _resX; ++x) {
                    *(_gridVertexIt++) = vertices_[x*_resY + y].pos();
                    if (x < _resX-1) {
                    *(_gridIndexIt++) = i;
                        *(_gridIndexIt++) = i+1;
                    }
                    ++i;
                }
            }

            for (size_t x = 0; x < _resX; x += subdivisions()) {
                for (size_t y = 0; y < _resY; ++y) {
                    *(_gridVertexIt++) = vertices_[x*_resY + y].pos();
                    if (y < _resY-1) {
                    *(_gridIndexIt++) = i;
                        *(_gridIndexIt++) = i+1;
                    }
                    ++i;
                }
            }

            vertexVbo_.bufferStaticArray(vertices_);
            indexVbo_.bufferStaticElementArray(indices_);

            gridVertexVbo_.bufferStaticArray(gridVertices_);
            gridIndexVbo_.bufferStaticElementArray(gridIndices_);
        }
    }
}

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

#include <omni/proj/Frustum.h>

#include <cmath>
#include <omni/proj/Projector.h>

namespace omni {
    namespace proj {
        Frustum::Frustum(Projector const& _proj) : proj_(_proj) {}

        QVector3D Frustum::eye() const {
            return proj_.pos();
        }

        QVector3D Frustum::topLeft() const {
            return QVector3D(1.0, rect().left(), rect().top());
        }

        QVector3D Frustum::topRight() const {
            return QVector3D(1.0, rect().right(), rect().top());
        }

        QVector3D Frustum::bottomLeft() const {
            return QVector3D(1.0, rect().left(), rect().bottom());
        }

        QVector3D Frustum::bottomRight() const {
            return QVector3D(1.0, rect().right(), rect().bottom());
        }

        QVector3D Frustum::topLeft(QMatrix4x4 const& _m) const {
            return _m * topLeft() - _m.column(3).toVector3D();
        }

        QVector3D Frustum::topRight(QMatrix4x4 const& _m) const {
            return _m * topRight() - _m.column(3).toVector3D();
        }

        QVector3D Frustum::bottomLeft(QMatrix4x4 const& _m) const {
            return _m * bottomLeft() - _m.column(3).toVector3D();
        }

        QVector3D Frustum::bottomRight(QMatrix4x4 const& _m) const {
            return _m * bottomRight() - _m.column(3).toVector3D();
        }

        QRectF Frustum::rect() const {
            qreal _a      = proj_.fov().radians() * 0.5;
            qreal _width  = tan(_a);
            qreal _height = _width / proj_.aspectRatio();

            return QRectF(
                QPointF(-_width, _height * (proj_.keystone() + 1.0)),
                QPointF(_width,  _height * (proj_.keystone() - 1.0)));
        }

        QRectF Frustum::rect(float _near, float _far) const {
            QRectF _rect = rect();
            return QRectF(_rect.topLeft() * _near,_rect.bottomRight() * _near);
        }
    }
}

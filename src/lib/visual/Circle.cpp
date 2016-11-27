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

#include <omni/visual/Circle.h>
#include <omni/visual/util.h>

namespace omni {
  namespace visual {
    Circle::Circle()
    {
      update();
    }

    void Circle::drawLine(QPointF const& _pos, float _rX, float _rY) const
    {
      withCurrentContext([&](QOpenGLFunctions& _) {
        glPushMatrix();
        glScalef(_rX, _rY, 1.0);
        glTranslatef(_pos.x() / _rX, _pos.y() / _rY, 0.0);
        glEnableClientState(GL_VERTEX_ARRAY);
        _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());

        glVertexPointer(2, GL_FLOAT, 0, nullptr);
        _.glDrawArrays(GL_LINE_LOOP, 1, numberSegments);

        _.glBindBuffer(GL_ARRAY_BUFFER,         0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
      });
    }

    void Circle::drawFill(QPointF const& _pos, float _rX, float _rY) const
    {
      withCurrentContext([&](QOpenGLFunctions& _) {
        glPushMatrix();
        glLoadIdentity();
        glScalef(_rX, _rY, 1.0);
        glTranslatef(_pos.x() / _rX, _pos.y() / _rY, 0.0);
        glEnableClientState(GL_VERTEX_ARRAY);
        _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());

        glVertexPointer(2, GL_FLOAT, 0, nullptr);
        _.glDrawArrays(GL_TRIANGLE_FAN, 0, numberSegments + 2);

        _.glBindBuffer(GL_ARRAY_BUFFER,         0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
      });
    }

    void Circle::update() {
      vertexVbo_.gen();
      glEnableClientState(GL_VERTEX_ARRAY);

      vertices_[0] = QVector2D(0.0, 0.0);
      util::for_each_circle_point(numberSegments, 1.0,
                                  [&](size_t i, const QPointF& _p)
      {
        vertices_[i + 1] = QVector2D(_p);
      });
      vertices_[numberSegments + 1] = vertices_[1];

      vertexVbo_.bufferStaticArray(vertices_);
    }
  }
}

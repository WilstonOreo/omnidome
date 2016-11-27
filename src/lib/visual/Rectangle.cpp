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

#include <omni/visual/Rectangle.h>

#include <omni/visual/util.h>

#include <QOpenGLFunctions>

namespace omni {
  namespace visual {
    void Rectangle::draw(float _left, float _right, float _top, float _bottom)
    {
      glBegin(GL_QUADS);
      {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(_left, _bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(_right, _bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(_right, _top);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(_left, _top);
      }
      glEnd();
    }

    void Rectangle::draw(QSize const& _size) {
      withCurrentContext([&](QOpenGLFunctions& _) {
        glBegin(GL_QUADS);
        {
          int _w = _size.width();
          int _h = _size.height();
          glTexCoord2f(0.0f, _h);
          glVertex2f(-0.5, -0.5);
          glTexCoord2f(_w, _h);
          glVertex2f(0.5, -0.5);
          glTexCoord2f(_w, 0.0f);
          glVertex2f(0.5, 0.5);
          glTexCoord2f(0.0f, 0.0f);
          glVertex2f(-0.5, 0.5);
        }
        glEnd();
      });
    }

    void Rectangle::draw(QRectF const& _rect)
    {
      Rectangle::draw(_rect.left(), _rect.right(), _rect.top(), _rect.bottom());
    }

    void Rectangle::drawFlipped(float _left,
                                float _right,
                                float _top,
                                float _bottom)
    {
      glBegin(GL_QUADS);
      {
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(_left, _bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(_right, _bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(_right, _top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(_left, _top);
      }
      glEnd();
    }

    void Rectangle::drawFlipped(QRectF const& _rect)
    {
      Rectangle::drawFlipped(_rect.left(), _rect.right(),
                             _rect.top(), _rect.bottom());
    }
  }
}

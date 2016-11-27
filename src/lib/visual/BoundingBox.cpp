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

#include <omni/visual/BoundingBox.h>

#include <omni/visual/util.h>
#include <QDebug>

namespace omni {
  namespace visual {
    BoundingBox::BoundingBox(omni::Box const& _box) :
      box_(_box)
    {
      update();
    }

    BoundingBox::~BoundingBox()
    {}

    void BoundingBox::update()
    {}

    void BoundingBox::draw(omni::Box const& _box)
    {
      float x            = _box.min().x(), y = _box.min().y(), z = _box.min().z();
      float xs           = _box.max().x(), ys = _box.max().y(),
            zs           = _box.max().z();
      GLuint _visualMode = GL_LINE_LOOP;

      glBegin(_visualMode);
      {
        // top side
        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord2f(0, 0);
        glVertex3f(x, ys, z);
        glTexCoord2f(1, 0);
        glVertex3f(xs, ys, z);
        glTexCoord2f(1, 1);
        glVertex3f(xs, ys, zs);
        glTexCoord2f(0, 1);
        glVertex3f(x, ys, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // bottom side
        glNormal3f(0.0, -1.0, 0.0);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, z);
        glTexCoord2f(1, 0);
        glVertex3f(xs, y, z);
        glTexCoord2f(1, 1);
        glVertex3f(xs, y, zs);
        glTexCoord2f(0, 1);
        glVertex3f(x, y, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // east side
        glNormal3f(0.0, 0.0, -1.0);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, z);
        glTexCoord2f(1, 0);
        glVertex3f(xs, y, z);
        glTexCoord2f(1, 1);
        glVertex3f(xs, ys, z);
        glTexCoord2f(0, 1);
        glVertex3f(x, ys, z);
      }
      glEnd();
      glBegin(_visualMode);

      // west side
      {
        glNormal3f(0.0, 0.0, 1.0);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, zs);
        glTexCoord2f(1, 0);
        glVertex3f(xs, y, zs);
        glTexCoord2f(1, 1);
        glVertex3f(xs, ys, zs);
        glTexCoord2f(0, 1);
        glVertex3f(x, ys, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // north side
        glNormal3f(-1.0, 0.0, 0.0);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, z);
        glTexCoord2f(1, 0);
        glVertex3f(x, y, zs);
        glTexCoord2f(1, 1);
        glVertex3f(x, ys, zs);
        glTexCoord2f(0, 1);
        glVertex3f(x, ys, z);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // south side
        glNormal3f(1.0, 0.0, 0.0);
        glTexCoord2f(0, 0);
        glVertex3f(xs, y, z);
        glTexCoord2f(1, 0);
        glVertex3f(xs, y, zs);
        glTexCoord2f(1, 1);
        glVertex3f(xs, ys, zs);
        glTexCoord2f(0, 1);
        glVertex3f(xs, ys, z);
      }
      glEnd();
    }

    void BoundingBox::draw() const
    {
      draw(box_);
    }
  }
}

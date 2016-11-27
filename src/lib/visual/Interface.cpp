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

#include <omni/visual/Interface.h>

#include <QColor>
#include <QVector3D>

namespace omni {
  namespace visual {
    void Interface::vertex3(QVector3D const& _v)
    {
      glVertex3f(_v.x(), _v.y(), _v.z());
    }

    void Interface::visualLine(QVector3D const& _from, QVector3D const& _to)
    {
      glBegin(GL_LINES);
      vertex3(_from);
      vertex3(_to);
      glEnd();
    }

    void Interface::color(QColor _color, float _alpha)
    {
      glColor4f(_color.red() / 255.0, _color.green() / 255.0,
                _color.blue() / 255.0, _alpha);
    }
  }
}

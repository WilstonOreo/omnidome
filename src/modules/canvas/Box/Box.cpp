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

#include "Box.h"
#include "BoxWidget.h"

#include <omni/visual/Box.h>

namespace omni
{
  namespace canvas
  {
    Box::Box() : center_(0.0,0.0,0.0)
    {
      setSize(QVector3D(5,5,5));
    }

    Box::~Box()
    {
    }

    void Box::draw() const
    {
        glPushMatrix();
        {
            vizBox_.draw();
        }
        glPopMatrix();
    }

    void Box::update()
    {
      vizBox_.update();
    }

    QVector3D Box::size() const
    {
      return this->bounds_.size();
    }

    void Box::setSize(QVector3D const& _s)
    {
      bounds_.setMinMax(
          QVector3D(-_s.x()*0.5,-_s.y()*0.5,0.0),
          QVector3D( _s.x()*0.5, _s.y()*0.5,_s.z()));
    }

    void Box::fromStream(QDataStream& _stream)
    {
      QVector3D _size;
      _stream >> _size;
      setSize(_size);
    }

    void Box::toStream(QDataStream& _stream) const
    {
      _stream << size();
    }

    QWidget* Box::widget() {
        return new ui::canvas::Box(this);
    }

    QMatrix4x4 Box::matrix() const {
        QMatrix4x4 _mat = Envelope::matrix();
        _mat.translate(QVector3D(0.0,0.0,bounds().center().z()));
        _mat.scale(QVector3D(size().x(),size().y(),size().z()));
        return _mat;
    }
  }
}

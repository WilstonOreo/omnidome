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
      vizBox_.draw();
    }

    void Box::update()
    {
      vizBox_.update();
    }

    QVector3D Box::size() const
    {
      return this->bounds_.size();
    }

    QVector3D Box::center() const {
        auto _c = bounds().center();
        return QVector3D(_c.x(),_c.y(),bounds().min().z()) + center_;
    }

    void Box::setCenter(QVector3D const& _c) {
        center_ =_c;
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

    QMatrix4x4 Box::matrix() const
    {
      QMatrix4x4 _m;
      _m.translate(center_ + QVector3D(0.0,0.0,bounds().center().z()));
      _m *= angles_.matrix();
      _m.scale(size());
      return _m;
    }

    QWidget* Box::widget() {
        return new ui::canvas::Box(this);
    }
  }
}

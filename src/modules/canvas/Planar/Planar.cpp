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

#include "Planar.h"
#include "PlanarWidget.h"

#include <QVector2D>

namespace omni
{
  namespace canvas
  {
    Planar::Planar()
    {
    }

    Planar::~Planar()
    {
    }

    void Planar::draw() const
    {
      glPushMatrix();

      glLoadIdentity();
      glTranslatef(center_.x(),center_.y(),center_.z());
      glMultMatrixf(angles_.matrix().constData());

      glBegin(GL_QUADS);
      float _h = width_ * 0.5;
      float _w = height_ * 0.5;
      glVertex3f(-_w, -_h, 0.0);
      glVertex3f( _w, -_h, 0.0);
      glVertex3f( _w,  _h, 0.0);
      glVertex3f(-_w,  _h, 0.0);
      glEnd();
      glPopMatrix();
    }

    void Planar::drawAux() const
    {
    }

    float Planar::width() const
    {
      return width_;
    }

    void Planar::setWidth(float _width)
    {
      width_=_width;
    }

    float Planar::height() const
    {
      return height_;
    }

    void Planar::setHeight(float _height)
    {
      height_=_height;
    }

    EulerAngles& Planar::angles()
    {
      return angles_;
    }

    EulerAngles const& Planar::angles() const
    {
      return angles_;
    }

    QVector3D Planar::center() const
    {
      return center_;
    }

    void Planar::setCenter(QVector3D const& _center)
    {
      center_=_center;
    }

    omni::Box Planar::bounds() const
    {
      QVector2D _vec(width_,height_);
      _vec *= 0.5;
      float _radius = _vec.length();
      QVector3D _v(_radius,_radius,_radius);
      return Box(-_v,_v);
    }

    void Planar::fromStream(QDataStream& _stream)
    {
      _stream >> width_ >> height_ >> center_ >> angles_;
    }

    void Planar::toStream(QDataStream& _stream) const
    {
      _stream << width_ << height_ << center_ << angles_;
    }

    QWidget* Planar::widget() {
        return new ui::canvas::Planar(this);
    }
  }
}

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

#include <omni/proj/FreeSetup.h>

#include <QMatrix4x4>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    FreeSetup::FreeSetup() :
      pos_(0.0,0.0,0.0)
    {
    }

    void FreeSetup::toPropertyMap(PropertyMap& _map) const
    {
      AngleSetup::toPropertyMap(_map);
      _map("pos",pos_);
    }

    void FreeSetup::fromPropertyMap(PropertyMap const& _map)
    {
      AngleSetup::fromPropertyMap(_map);
      _map.get("pos",pos_);
    }

    void FreeSetup::setup(Projector& _proj)
    {
      QMatrix4x4 _m;
      _m.translate(pos_);
      _m *= EulerAngles::matrix();
      _proj.setMatrix(_m);
    }

    QVector3D FreeSetup::pos() const
    {
      return pos_;
    }

    void FreeSetup::setPos(QVector3D const& _pos)
    {
      pos_=_pos;
    }

    void FreeSetup::setPos(float _x, float _y, float _z)
    {
      pos_ = QVector3D(_x,_y,_z);
    }

    void FreeSetup::scale(qreal _scale) {
      pos_ *= _scale;
    }
  }
}

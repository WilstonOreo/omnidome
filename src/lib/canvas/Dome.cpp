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

#include <omni/canvas/Dome.h>

#include <omni/serialization/PropertyMap.h>

namespace omni {
  namespace canvas {
    Dome::Dome()
    {
    }

    Dome::~Dome()
    {}

    void Dome::draw() const
    {
      Envelope::draw();

      this->sphere_.draw();
    }

    void Dome::drawAux() const
    {}

    qreal Dome::radius() const
    {
      return sphere_.radius();
    }

    void Dome::setRadius(qreal _radius)
    {
      sphere_.setRadius(_radius);
      needsUpdate_ = true;
      update();
    }

    qreal Dome::diameter() const
    {
      return radius() * 2.0;
    }

    void Dome::setDiameter(qreal _diameter)
    {
      setRadius(_diameter * 0.5);
    }

    Box Dome::bounds() const
    {
      return bounds_;
    }

    void Dome::update()
    {
      if (!this->needsUpdate_) return;

      auto _r = radius();
      QVector3D _vr(_r, _r, _r);
      this->bounds_ = Box(-_vr, _vr);
      sphere_.update();
    }

    void Dome::toPropertyMap(PropertyMap& _map) const
    {
      Envelope::toPropertyMap(_map);
      _map("radius", radius());
    }

    void Dome::fromPropertyMap(PropertyMap const& _map)
    {
      Envelope::fromPropertyMap(_map);
      qreal _radius = _map.getValue<qreal>("radius", qreal(5.0));
      setRadius(_radius);
    }
  }
}

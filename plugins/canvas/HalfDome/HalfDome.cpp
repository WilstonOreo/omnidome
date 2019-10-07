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

#include "HalfDome.h"
#include "HalfDomeWidget.h"

#include <omni/ui/CanvasParameters.h>

namespace omni
{
  namespace canvas
  {
    HalfDome::HalfDome()
    {
      this->sphere_.setTexCoordsMode(visual::Sphere::FISHEYE);
      this->sphere_.setBottom(0.0);
      setDiameter(10.0);
    }

    ui::CanvasParameters* HalfDome::widget() {
        return new ui::canvas::HalfDome;
    }
  }
}

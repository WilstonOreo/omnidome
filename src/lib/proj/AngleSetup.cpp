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

#include <omni/proj/AngleSetup.h>

#include <QMatrix4x4>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    AngleSetup::AngleSetup()
    {
        setPitch(30.0);
    }

    AngleSetup::~AngleSetup()
    {
    }

    void AngleSetup::toPropertyMap(PropertyMap& _map) const
    {
      _map("angles",static_cast<EulerAngles const&>(*this));
    }

    void AngleSetup::fromPropertyMap(PropertyMap const& _map)
    {
      _map.get("angles",static_cast<EulerAngles&>(*this));
    }

    void AngleSetup::setup(Projector& _proj)
    {
      _proj.setMatrix(EulerAngles::matrix());
    }
  }
}

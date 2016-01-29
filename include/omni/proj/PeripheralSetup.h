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

#ifndef OMNI_PROJ_PERIPHERALSETUP_H_
#define OMNI_PROJ_PERIPHERALSETUP_H_

#include "AngleSetup.h"

namespace omni
{
  namespace proj
  {
    class PeripheralSetup : public AngleSetup
    {
    public:
      OMNI_REGISTER_CLASS(SetupFactory,PeripheralSetup)

      PeripheralSetup();

      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);
      void setup(Projector&);

      Angle deltaYaw() const;
      void setDeltaYaw(Angle);

      qreal distanceCenter() const;
      void setDistanceCenter(qreal);

      qreal towerHeight() const;
      void setTowerHeight(qreal);

      qreal shift() const;
      void setShift(qreal);

    private:
      Angle deltaYaw_;
      qreal distanceCenter_;
      qreal towerHeight_;
      qreal shift_;
    };
  }
}

#endif /* OMNI_PROJ_PERIPHERALSETUP_H_ */

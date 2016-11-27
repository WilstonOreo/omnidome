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

#ifndef OMNI_PROJ_PERIPHERALSETUP_H_
#define OMNI_PROJ_PERIPHERALSETUP_H_

#include "AngleSetup.h"

namespace omni {
  namespace proj {
    /**@brief Setup in which projector rotates with yaw angle around center point
     **/
    class PeripheralSetup : public AngleSetup {
      public:
        OMNI_REGISTER_CLASS(SetupFactory, PeripheralSetup)

        PeripheralSetup();

        /// Serialize to stream
        void  toPropertyMap(PropertyMap&) const;

        /// Deserialize from stream
        void  fromPropertyMap(PropertyMap const&);

        /// Setup projector with scene scale factor
        void  setup(Projector&);

        /// Delta yaw angle from rotating yaw angle
        Angle deltaYaw() const;

        /// Delta yaw angle from rotating yaw angle
        void setDeltaYaw(Angle);

        /// Return distance from center
        qreal distanceCenter() const;

        /// Set distance to center
        void setDistanceCenter(qreal);

        /// Return tower height (z position)
        qreal towerHeight() const;

        /// Set tower height (z position of projector)
        void setTowerHeight(qreal);

        /// Distance to centric line
        qreal shift() const;

        /// Set shift (distance to centric line)
        void setShift(qreal);

        /// Scale projector setup by factor
        void scale(qreal);

      private:
        Angle deltaYaw_ = 0.0;
        qreal distanceCenter_ = 0.4;
        qreal towerHeight_ = 0.2;
        qreal shift_ = 0.0;
    };
  }
}

#endif /* OMNI_PROJ_PERIPHERALSETUP_H_ */

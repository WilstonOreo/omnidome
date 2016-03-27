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

#ifndef OMNI_PROJ_ANGLESETUP_H_
#define OMNI_PROJ_ANGLESETUP_H_

#include "Setup.h"
#include <omni/EulerAngles.h>

namespace omni {
  namespace proj {
    /// Basic projector setup with three euler angles and no positioning offsets
    class AngleSetup :
      public Setup,
      public EulerAngles // Inherit from this to get yaw, pitch, roll angle
    {
      public:
        OMNI_REGISTER_CLASS(SetupFactory, AngleSetup)

        AngleSetup();
        ~AngleSetup();

        virtual void toStream(QDataStream&) const;
        virtual void fromStream(QDataStream&);
        virtual void setup(Projector&);
    };
  }
}

#endif /* OMNI_PROJ_ANGLESETUP_H_ */

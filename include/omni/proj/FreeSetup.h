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

#ifndef OMNI_PROJ_FREESETUP_H_
#define OMNI_PROJ_FREESETUP_H_

#include <QVector3D>
#include "AngleSetup.h"

namespace omni {
  namespace proj {
    /// Free projector setup with euler angles and positioning
    class FreeSetup :
      public AngleSetup {
      public:
        OMNI_REGISTER_CLASS(SetupFactory, FreeSetup)

        FreeSetup();

        /// Setup projector with scene size
        void      setup(Projector&);

        void      toPropertyMap(PropertyMap&) const;
        void      fromPropertyMap(PropertyMap const&);

        /// Return projector position
        QVector3D pos() const;

        /// Set position from QVector3D
        void      setPos(QVector3D const&);

        /// Set position from x,y,z position values
        void      setPos(float _x,
                         float _y,
                         float _z);

        /// Scale by factor
        void      scale(qreal);
      private:
        QVector3D pos_;
    };
  }
}

#endif /* OMNI_PROJ_FREESETUP_H_ */

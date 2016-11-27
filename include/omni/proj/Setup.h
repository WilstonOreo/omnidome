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

#ifndef OMNI_PROJ_SETUP_H_
#define OMNI_PROJ_SETUP_H_

#include <vector>
#include <omni/TypeIdInterface.h>
#include <omni/serialization/PropertyMapSerializer.h>

namespace omni {
  namespace proj {
    class Projector;

    /// Interface for a projector setup
    class Setup :
      public PropertyMapSerializer,
      public TypeIdInterface {
      public:
        /// Setup projector with scene size
        virtual void setup(Projector&) = 0;

        inline virtual bool isMultiSetup() const {
          return false;
        }

        inline virtual void setup(Projector& _proj, qreal _scale) {
          setup(_proj);
          scale(_scale);
        }

        /// Scale projector setup by factor, e.g. to adapt defaults to scene size
        virtual void scale(qreal _factor) = 0;

        virtual bool flipped() const = 0;
    };

    /// Typedef for our factory
    typedef AbstractFactory<Setup>SetupFactory;
  }

  typedef proj::Setup ProjectorSetup;
}

#endif /* OMNI_PROJ_SETUP_H_ */

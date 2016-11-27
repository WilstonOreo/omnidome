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

#ifndef OMNI_INPUT_CONTROLLER_H_
#define OMNI_INPUT_CONTROLLER_H_

#include <unordered_map>
#include "Interface.h"

namespace omni {
  namespace input {
    /**@brief Input controller controls which inputs are currently active
       @detail Implemented as a singleton
    **/
    class Controller {
      public:
        static Controller* instance();

        bool isActive(input::Interface *) const;
        int activeCount(input::Interface *) const;

        void add(input::Interface*);

        void remove(input::Interface*);

        /// Activate input
        void activate(input::Interface *);

        /// Deactivate input
        void deactivate(input::Interface *);

      private:
        Controller();
        ~Controller();

        static Controller* instance_;

        std::unordered_map<input::Interface *, int> usage_;
    };
  }
}

#endif /* OMNI_INPUT_CONTROLLER_H_ */

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

#include <omni/input/Controller.h>

namespace omni {
  namespace input {
    Controller* Controller::instance_ = nullptr;

    Controller::Controller() {

    }

    Controller::~Controller() {
    }

    Controller* Controller::instance() {
      if (!instance_) {
        instance_ = new Controller;
      }
      return instance_;
    }

    bool Controller::isActive(input::Interface *_input) const {
      return activeCount(_input) > 0;
    }

    int Controller::activeCount(input::Interface *_input) const {
      if (usage_.count(_input) == 0) return 0;

      return usage_.at(_input);
    }

    void Controller::activate(input::Interface *_input) {
      if (!_input) return;

      if (!isActive(_input)) {
        // Activate input if it not in usage list
        _input->activate();

        // Set usage count to 1 initially
        usage_[_input] = 1;
      } else {
        /// Increment usage count
        ++usage_[_input];
      }
    }

    void Controller::deactivate(input::Interface *_input) {
      if (usage_.count(_input) == 0) return;
      if (usage_[_input] == 0) return;

      --usage_[_input];

      if (!isActive(_input)) {
        _input->deactivate();
      }
    }

    void Controller::add(input::Interface* _input) {
      if (usage_.count(_input) > 0) return;

      usage_[_input] = 0; // Do not activate the input
    }

    void Controller::remove(input::Interface* _input) {
      if (usage_.count(_input) == 0) return;
      deactivate(_input);
      usage_.erase(_input);
    }


  }
}

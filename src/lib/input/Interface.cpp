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

#include <omni/input/Interface.h>

namespace omni {
    namespace input {
        Interface::~Interface()
        {}

        Interface * Interface::addInput(QString const& _id, Interface *_i) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (children_.count(_id) != 0) {
                return nullptr;
            }
            children_[_id].reset(_i);
            return children_[_id].get();
        }

        void Interface::removeInput(QString const& _id) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            children_.erase(_id);
        }

        Interface* Interface::getInput(QString const& _id) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (children_.count(_id) == 0) return nullptr;

            return children_.at(_id).get();
        }

        Interface const* Interface::getInput(QString const& _id) const {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (children_.count(_id) == 0) return nullptr;

            return children_.at(_id).get();
        }

        Interface::children_type& Interface::children() {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            return children_;
        }

        Interface::children_type const& Interface::children() const {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            return children_;
        }
    }
}

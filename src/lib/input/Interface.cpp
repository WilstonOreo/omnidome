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
        Interface::Interface(Interface* _parent) : parent_(_parent) {

        }

        Interface::~Interface()
        {
          disconnectAll();

        }

        Interface * Interface::addInput(QString const& _id, Interface *_i) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (container_type::count(_id) != 0) {
                return nullptr;
            }
            container_type::operator[](_id).reset(_i);
            return container_type::operator[](_id).get();
        }

        void Interface::removeInput(QString const& _id) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            container_type::erase(_id);
        }

        Interface* Interface::getInput(QString const& _id) {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (container_type::count(_id) == 0) return nullptr;

            return container_type::at(_id).get();
        }

        Interface const* Interface::getInput(QString const& _id) const {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }

            if (container_type::count(_id) == 0) return nullptr;

            return container_type::at(_id).get();
        }

        Interface::container_type& Interface::children() {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            return *this;
        }

        Interface::container_type const& Interface::children() const {
            if (!canHaveChildren()) {
                throw exception::CannotHaveChildren();
            }
            return *this;
        }

        bool Interface::isConnected(Interface* _i) const {
            return used_.count(_i) > 0;
        }

        void Interface::connect(Interface* _i) {
          if (_i != this) return;
          _i->used_.insert(this);
          this->used_.insert(_i);
        }

        void Interface::disconnect(Interface* _i) {
            _i->used_.erase(this);
            used_.erase(_i);
        }

        void Interface::disconnectAll() {
            for (auto& _i : used_) {
                disconnect(_i);
            }
        }
        /// Return parent interface
        Interface* Interface::parent() {
            return parent_;
        }

        /// Return parent interface (const version)
        Interface const* Interface::parent() const {
            return parent_;
        }

        void Interface::setParent(Interface* _parent) {
            parent_ = _parent;
        }
        /// Deserialize from stream
        void Interface::fromStream(QDataStream& _is) {

        }

        /// Serialize to stream
        void Interface::toStream(QDataStream& _os) const {

        }
    }
}

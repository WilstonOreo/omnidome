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

#include <omni/util.h>
#include <omni/serialization/pointer.h>
#include <omni/serialization/container.h>
#include <omni/ui/InputPreview.h>

namespace omni {
  namespace input {
    Interface::Interface(Interface const *_parent) : parent_(_parent) {}

    Interface::~Interface()
    {}

    Input * Interface::addInput(QString const& _id, Id const& _typeId)
    {
      std::unique_ptr<Input> _input(Factory::create(_typeId, this));

      if (!_input) return nullptr;

      _input->parent_ = this;
      children()[_id] = std::move(_input);
      return children().at(_id).get();
    }

    Interface * Interface::addInput(QString const& _id, Interface *_i) {
      if (!canHaveChildren()) {
        throw exception::CannotHaveChildren();
      }

      if (container_type::count(_id) != 0) {
        return nullptr;
      }
             container_type::operator[](_id).reset(_i);
      _i->parent_ = this;

      return container_type::operator[](_id).get();
    }

    void Interface::removeInput(QString const& _id) {
      if (!canHaveChildren()) {
        throw exception::CannotHaveChildren();
      }
      this->operator[](_id)->free();
      container_type::erase(_id);
    }

    Interface * Interface::getInput(QString const& _id) {
      if (!canHaveChildren()) {
        throw exception::CannotHaveChildren();
      }

      if (container_type::count(_id) == 0) return nullptr;

      return container_type::at(_id).get();
    }

    Interface const * Interface::getInput(QString const& _id) const {
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

    QString Interface::getId(Interface const *_i) const {
      if (_i->parent() != this) return "";

      for (auto& _child : children_) {
        if (_i == _child.second.get()) {
          return _child.first;
        }
      }
      return QString("");
    }

    QString Interface::path() const {
      if (!parent()) return "/";

      auto   *_parent = parent();
      QString _path;

      while (_parent != nullptr) {
        _path   = QString("/") + _parent->getId(this) + _path;
        _parent = _parent->parent();
      }
      return _path;
    }

    /// Get interface by absolute path
    Interface * Interface::getByPath(QString const& _path) {}

    /// Get interface by absolute path (const version)
    Interface const * Interface::getByPath(QString const& _path) const {}

    /// Return parent interface (const version)
    Interface const * Interface::parent() const {
      return parent_;
    }

    void Interface::setParent(Interface *_parent) {
      parent_ = _parent;
    }

    /// Deserialize from stream
    void Interface::fromStream(QDataStream& _is) {
      using namespace omni::util;
      clear();

      int _size = deserializeReturn<int>(_is, 0);

      if (canHaveChildren()) {
        // Deserialize map of inputs
        for (int i = 0; i < _size; ++i)
        {
          auto _id = deserializeReturn<QString>(_is);
          deserializePtr(_is, [&](Id const& _typeId) ->
                         input::Interface *
          {
            return addInput(_id, _typeId);
          });
        }
      }
    }

    QWidget * Interface::widget() {
      return new ui::InputPreview(this);
    }

    /// Serialize to stream
    void Interface::toStream(QDataStream& _os) const {
      using namespace omni::util;

      if (canHaveChildren()) {
        // serialize map of inputs
        serialize(_os, int(children().size()));

        for (auto& _idInput : (*this))
        {
          auto& _id    = _idInput.first;
          auto& _input = _idInput.second;
          serialize(_os, _id);
          serialize(_os, _input.get());
        }
      } else {
        serialize(_os, int(0));
      }
    }
  }
}

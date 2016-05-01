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
#include <omni/input/Controller.h>

#include <omni/util.h>
#include <omni/serialization/pointer.h>
#include <omni/serialization/container.h>
#include <omni/ui/InputPreview.h>

namespace omni {
  namespace input {
    Interface::Interface(Interface const *_parent) : parent_(_parent) {
      Controller::instance()->add(this);
    }

    Interface::~Interface()
    {
      updatedCallbacks_.clear();
      /// Removing input from global controller also deactivates it
      Controller::instance()->remove(this);
    }

    Input * Interface::addInput(QString const& _id, Id const& _typeId)
    {
      std::unique_ptr<Input> _input(Factory::create(_typeId, this));

      if (!_input) return nullptr;

      _input->parent_ = this;
      container_type::operator[](_id) = std::move(_input);
      return container_type::at(_id).get();
    }

    Interface * Interface::addInput(QString const& _id, Interface *_i) {
      if (container_type::count(_id) != 0) {
        return nullptr;
      }
             container_type::operator[](_id).reset(_i);
      _i->parent_ = this;

      return container_type::operator[](_id).get();
    }

    void Interface::removeInput(QString const& _id) {
      container_type::erase(_id);
    }

    Interface * Interface::getInput(QString const& _id) {
      if (container_type::count(_id) == 0) return nullptr;

      return container_type::at(_id).get();
    }

    Interface const * Interface::getInput(QString const& _id) const {
      if (container_type::count(_id) == 0) return nullptr;

      return container_type::at(_id).get();
    }

    Interface::container_type const& Interface::children() const {
      return *this;
    }

    QString Interface::getId(Interface const *_i) const {
      if (_i->parent() != this) return "";

      for (auto& _child : *this) {
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

    /// Return parent interface (const version)
    Interface const * Interface::parent() const {
      return parent_;
    }

    void Interface::setParent(Interface *_parent) {
      parent_ = _parent;
    }


    QWidget * Interface::widget() {
      return new ui::InputPreview(this);
    }

    Interface::inputlist_type  Interface::getAllInputs() const {
      inputlist_type _inputs;
      Interface const* _root = this;
      while (_root->parent()) {
        _root = _root->parent();
      }
      getInputsRecurse(_root,_inputs,true);
      return _inputs;
    }

    void Interface::getInputsRecurse(
      Interface const* _root,
      inputlist_type& _list,
      bool _excludeThis) const {
      for (auto& _idChild : *this) {
        auto* _child = _idChild.second.get();
        if (!((_child == this) && _excludeThis)) {
          _list.push_back(_child);
        }

        getInputsRecurse(_child,_list,_excludeThis);
      }
    }

    /// Serialize to stream
    void Interface::toPropertyMap(PropertyMap& _map) const {
      using namespace omni::util;
      PropertyMap _children;
      for (auto& _idChild : children()) {
        OMNI_DEBUG << _idChild.first << _idChild.second.get();
        _children.put(_idChild.first,_idChild.second);
      }
      _map.put("children",_children);
    }

    /// Deserialize from stream
    void Interface::fromPropertyMap(PropertyMap const& _map) {
      using namespace omni::util;
      clear();
      PropertyMap _children = _map.getValue("children",PropertyMap());
      auto _ids = _children.ids();

      for (auto& _id : _ids) {
        _children.getPtr(_id,[&](Id const& _typeId) ->
                         input::Interface *
          {
            return addInput(_id, _typeId);
          });
      }
    }
  }
}

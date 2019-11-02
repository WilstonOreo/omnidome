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

#include <omni/input/List.h>

#include <algorithm>
#include <omni/util.h>
#include <omni/serialization/traits.h>
#include <omni/serialization/pointer.h>
#include <omni/serialization/container.h>

namespace omni {
  namespace input {
    void List::clear()
    {

    }

    Input* List::operator[](int index) const
    {
      return getInput(index);
    }

    void List::fromPropertyMap(PropertyMap const& _map)
    {
      clear();
      QDataStream _stream(_map.data("inputs"));
      uint32_t _size = deserializeReturn<uint32_t>(_stream, 0);
      for (uint32_t i = 0; i < _size; ++i) {
          deserializePtr(_stream,[&](Id const& _typeId) -> Input* {
            return addInput(_typeId);
          });
      }
      _map("currentIndex",currentIndex_);
      setCurrentIndex(currentIndex_);
    }

    void List::toPropertyMap(PropertyMap& _map) const {
      _map("currentIndex",currentIndex_); 
      _map("inputs",static_cast<container_type const&>(*this));
    }

    Input* List::current() const
    {
       return this->operator[](currentIndex_);
    }

    Input * List::addInput(Id const& _typeId)
    {
      std::unique_ptr<Input> _input(Input::factory().create(_typeId));

      if (!_input) return nullptr;

      container_type::emplace_back(std::move(_input));
      return container_type::back().get();
    }

    Input* List::addInput(Input*_input) {
      container_type::emplace_back(_input);
      return container_type::back().get();
    }

    void List::removeInput(int index) {
      if (!validIndex(index)) return;

      container_type::erase(container_type::begin() + index);
      if (!validIndex(index)) {
        setCurrentIndex(-1);
      }
    }

    Input* List::getInput(int index) const {
      if (!validIndex(index)) return nullptr;
      return container_type::at(index).get();
    }

    int List::currentIndex() const
    {
      return currentIndex_;
    }

    void List::setCurrentIndex(int index)
    {
      /// Deactivate current input
      if (current()) {
        current()->deactivate();
      }

      currentIndex_ = index;

      if (current()) {
        current()->update();
        current()->activate();
      }
    }

    bool List::validIndex(int index) const {
    
      return (index >= 0) && (index < size());
    }

    bool operator==(List const& _lhs, List const& _rhs)
    {
      if (_lhs.currentIndex_ != _rhs.currentIndex_) return false;

      if (_lhs.size() != _rhs.size()) return false;

      auto it = _lhs.begin();
      auto jt = _rhs.begin();

      for (; (it != _lhs.end()) && (jt != _rhs.end()); ++it, ++jt)
      {
        if (!(*it)->equal(jt->get())) return false;
      }
      return true;
    }
  }
}

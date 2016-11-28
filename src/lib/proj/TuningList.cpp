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

#include <omni/proj/TuningList.h>

#include <omni/util.h>
#include <omni/proj/Tuning.h>
#include <omni/visual/Tuning.h>
#include <omni/Session.h>
#include <omni/visual/Scene.h>
#include <omni/serialization/PropertyMap.h>

namespace omni
{
  namespace proj
  {
    TuningList::TuningList(Session const& _session) :
      session_(_session) {

    }

    Tuning* TuningList::add(bool _makeCurrent)
    {
      container_type::emplace_back(new Tuning(session_));
      auto* _tuning = container_type::back().get();
      if (_makeCurrent)
        setCurrentIndex(container_type::size()-1);

      // Assign virtual screen initially, this also sets up projector aspectRatio correctly
      _tuning->assignVirtualScreen();
      _tuning->projector().scale(session_.scene().size());

      return _tuning;
    }

    void TuningList::remove()
    {
      remove(currentIdx_);
    }

    Tuning* TuningList::current()
    {
      return validIndex(currentIdx_) ? container_type::at(currentIdx_).get() : nullptr;
    }
    Tuning const* TuningList::current() const
    {
      return validIndex(currentIdx_) ? container_type::at(currentIdx_).get() : nullptr;
    }

    void TuningList::setCurrentIndex(int _currentIdx)
    {
      if (_currentIdx < 0)
      {
        currentIdx_ = 0;
        return;
      }
      if (_currentIdx >= size())
      {
        currentIdx_ = size() - 1;
        return;
      }
      currentIdx_ = _currentIdx;
    }

    int TuningList::currentIndex() const
    {
      return currentIdx_;
    }

    void TuningList::remove(int _idx)
    {
      if (!validIndex(_idx)) return;
      container_type::erase(container_type::begin() + _idx);
    }

    void TuningList::clear()
    {
      container_type::clear();
      currentIdx_ = -1;
    }

    Tuning* TuningList::operator[](int _index)
    {
      return validIndex(_index) ?
        container_type::at(_index).get() : nullptr;
    }

    Tuning const* TuningList::operator[](int _index) const
    {
      return validIndex(_index) ?
        container_type::at(_index).get() : nullptr;
    }


    void TuningList::fromStream(QDataStream& _stream)
    {
      clear();
      PropertyMap _map;
      _stream >> _map;
      uint32_t _size = _map.getValue<uint32_t>("size",0);
      int _currentIdx = _map.getValue<int>("currentIndex",-1);

      for (int i = 0; i < _size; ++i)
      {
        auto* _tuning = add(false /* Dont modify current tuning index */);
        if (!_tuning) continue;
        _stream >> *_tuning;
      }
      setCurrentIndex(_currentIdx);
    }

    void TuningList::toStream(QDataStream& _stream) const
    {
      PropertyMap _map;
      _map("size",uint32_t(container_type::size()));
      _map("currentIndex",currentIdx_);
      _stream << _map;

      for (auto& _tuning : *this)
        _stream << *_tuning;
    }

    bool operator==(TuningList const& _lhs, TuningList const& _rhs)
    {
      // Lambda for testing if pointers of two tunings are equal
      auto _tuningsEqual = [](Tuning const* _a, Tuning const* _b) -> bool
      {
        return _a && _b // Test if pointer have same address
          ?
          // Derefence pointers and use equality operator to test equality
          ((*_a) == (*_b))
          :
          // Compare pointers and if test of pointer are not nullptrs
          (_a == _b);
      };

      return
        OMNI_TEST_MEMBER_EQUAL(currentIdx_) &&
        util::testPtrVectorEqual(_lhs,_rhs,_tuningsEqual);
    }

    bool TuningList::validIndex(int _idx) const
    {
      return (_idx >= 0) && (_idx < container_type::size());
    }
  }
}

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

#include <omni/input/List.h>

#include <algorithm>
#include <omni/util.h>
#include <omni/serialization/pointer.h>
#include <omni/serialization/container.h>

namespace omni
{
  namespace input
  {
    List::List(Interface* _parent) : Interface(_parent)
    {
    }

    void List::clear()
    {
      children().clear();
    }

    std::pair<QString,Input*> List::add(Id const& _typeId) {
        auto _id = generateId();
        return std::make_pair(_id,add(_id,_typeId));
    }

    Input* List::add(QString const& _id, Id const& _typeId)
    {
      std::unique_ptr<Input> _input(Factory::create(_typeId));
      if (!_input) return nullptr;

      children()[_id] = std::move(_input);
      return children().at(_id).get();
    }

    Input* List::operator[](QString const& _id)
    {
      return (children().count(_id) != 0) ?
        children().operator[](_id).get() :
        nullptr;
    }

    Input const* List::operator[](QString const& _id) const
    {
      return (children().count(_id) != 0) ?
        children().at(_id).get() :
        nullptr;
    }

    void List::remove(QString const& _id)
    {
      if (children().count(_id) == 0) return;

      this->operator[](_id)->free();
      children().erase(_id);

      if (children().empty())
        setCurrentId("");
    }

    void List::fromStream(QDataStream& _stream)
    {
      using namespace omni::util;
      clear();

      deserialize(_stream,currentId_);
      int _size = deserializeReturn<int>(_stream,0);

      // Deserialize map of inputs
      for (int i = 0; i < _size; ++i)
      {
        auto _id = deserializeReturn<QString>(_stream);
        deserializePtr(_stream,[&](Id const& _typeId) ->
            input::Interface*
        {
          return add(_id,_typeId);
        });
      }
      setCurrentId(currentId_);
    }

    void List::toStream(QDataStream& _stream) const
    {
      using namespace omni::util;

      serialize(_stream,currentId_);
      // serialize map of inputs
      serialize(_stream,int(children().size()));
      for (auto& _idInput : (*this))
      {
        auto& _id = _idInput.first;
        auto& _input = _idInput.second;
        serialize(_stream,_id);
        serialize(_stream,_input.get());
      }
    }

    Input const* List::current() const
    {
      return
        children().count(currentId_) > 0 ?
        children().at(currentId_).get() : nullptr;
    }

    Input* List::current()
    {
      return
        children().count(currentId_) > 0 ?
        children().at(currentId_).get() : nullptr;
    }

    QString List::currentId() const
    {
      return currentId_;
    }

    void List::setCurrentId(QString const& _id)
    {
      if (children().count(_id) == 0) {
        currentId_ = "";
        return;
      }

      currentId_ = _id;
    }

    bool operator==(List const& _lhs, List const& _rhs)
    {
      if (_lhs.currentId_ != _rhs.currentId_) return false;
      if (_lhs.size() != _rhs.size()) return false;

      auto it = _lhs.begin();
      auto jt = _rhs.begin();

      for (; (it != _lhs.end()) && (jt != _rhs.end()); ++it,++jt)
      {
        if (it->first != jt->first) return false;
        if (!it->second->equal(jt->second.get())) return false;
      }
      return true;
    }

    QString List::generateId() const {
        QString _id("0");
        for (size_t i = 0; i <= children().size(); ++i) {
            _id = QString("%1").arg(i);
            if (children().count(_id) == 0)
            {
                return _id;
            }
        }
        return _id;
    }
  }
}

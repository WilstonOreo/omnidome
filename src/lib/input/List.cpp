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
#include <omni/serialization/traits.h>
#include <omni/serialization/Interface.h>

namespace omni {
  namespace input {
    List::List(Interface const *_parent) : Interface(_parent)
    {}

    void List::clear()
    {
      Interface::clear();
    }

    std::pair<QString, Input *>List::addInput(Id const& _typeId) {
      auto _id = generateId();

      return std::make_pair(_id, input::Interface::addInput(_id, _typeId));
    }

    void List::removeInput(QString const& _id)
    {
      input::Interface::removeInput(_id);
      if (_id == currentId_) {
        setCurrentId("");
      }
    }

    Input * List::operator[](QString const& _id)
    {
      return Interface::getInput(_id);
    }

    Input const * List::operator[](QString const& _id) const
    {
      return Interface::getInput(_id);
    }

    void List::fromStream(QDataStream& _stream)
    {
      QString _currentId;
      deserialize(_stream, _currentId);
      input::Interface::fromStream(_stream);
      setCurrentId(_currentId);
    }

    void List::toStream(QDataStream& _stream) const {
      serialize(_stream, currentId_);
      input::Interface::toStream(_stream);
    }

    Input const * List::current() const
    {
       return getInput(currentId_);
    }

    Input * List::current()
    {
      return getInput(currentId_);
    }

    QString List::currentId() const
    {
      return currentId_;
    }

    void List::setCurrentId(QString const& _id)
    {
      if (getInput(_id) == nullptr) {
        currentId_ = "";
        return;
      }
      qDebug() << currentId_;

      /// Deactivate current input
      if (current()) {
        controller()->deactivate(current());
      }

      currentId_ = _id;
      qDebug() << currentId_;

      if (current()) {
        current()->update();
        controller()->activate(current());
      }
      qDebug() << current();
    }

    /// Return pointer to controller
    Controller *    List::controller() {
      return &controller_;
    }

    /// Return pointer to controller (const version)
    Controller const * List::controller() const {
      return &controller_;
    }

    bool operator==(List const& _lhs, List const& _rhs)
    {
      if (_lhs.currentId_ != _rhs.currentId_) return false;

      if (_lhs.size() != _rhs.size()) return false;

      auto it = _lhs.begin();
      auto jt = _rhs.begin();

      for (; (it != _lhs.end()) && (jt != _rhs.end()); ++it, ++jt)
      {
        if (it->first != jt->first) return false;

        if (!it->second->equal(jt->second.get())) return false;
      }
      return true;
    }

    QString List::generateId() const {
      QString _id("0");

      for (size_t i = 0; i <= numberOfChildren(); ++i) {
        _id = QString("%1").arg(i);

        if (getInput(_id) == nullptr)
        {
          return _id;
        }
      }
      return _id;
    }
  }
}

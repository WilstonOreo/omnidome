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
#ifndef OMNI_TYPEIDMEMORY_H_
#define OMNI_TYPEIDMEMORY_H_

#include <map>
#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <omni/TypeIdInterface.h>

namespace omni {
  /**@brief Helper class to store objects with a certain type id in QBuffer
     @detail Used in GUI for storing previously selected mapping or canvas types
   **/
  template<typename INTERFACE>
  class TypeIdMemory {
    public:
      /// Our interface type
      typedef INTERFACE interface_type;

      /// Clear memory
      void clear() {
        memory_.clear();
      }

      /// Store object with interface in memory
      void store(interface_type const *_t) {
        auto _id = _t->getTypeId();

        QBuffer _buf;
        /// Serialize to QByteArray
        _buf.open(QIODevice::WriteOnly);
        QDataStream _s(&_buf);
        _t->toStream(_s);
        memory_[_id] = _buf.data();
      }

      /// Restore object with interface in memory, if exists
      void restore(interface_type *_t) const {
        auto _id = _t->getTypeId();

        /// No exception is thrown
        if (memory_.count(_id) == 0) return;

        /// Deserialize from QByteArray
        QDataStream _s(memory_.at(_id));
        _t->fromStream(_s);
      }

    private:
      std::map<Id, QByteArray> memory_;
  };
}

#endif /* OMNI_TYPEIDMEMORY_H_ */

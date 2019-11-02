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

#ifndef OMNI_SERIALIZATION_INTERFACE_H_
#define OMNI_SERIALIZATION_INTERFACE_H_

#include <omni/TypeIdInterface.h>
#include <omni/util.h>

namespace omni {
  namespace serialization {
    /**@brief Abstract interface for serialization
     * @detail Holds two member functions for serialization and deserialization
     */
    class OMNI_EXPORT Interface {
      public:
        virtual ~Interface() {}

        /// Serialize to stream
        virtual void toStream(QDataStream&) const = 0;

        /// Deserialize from stream
        virtual void fromStream(QDataStream&) = 0;

        /// Optional virtual member method for testing equality of two
        // interfaces
        inline virtual bool equal(Interface const *_that) const
        {
          return true;
        }
    };
  }

  typedef serialization::Interface SerializationInterface;
}

#define OMNI_DECL_STREAM_OPERATORS(CLASS)                                 \
  inline QDataStream &operator>>(QDataStream & _stream, CLASS & _cls)     \
  {                                                                       \
    _cls.fromStream(_stream);                                             \
    return _stream;                                                       \
  }                                                                       \
  inline QDataStream& operator<<(QDataStream& _stream, CLASS const& _cls) \
  {                                                                       \
    _cls.toStream(_stream);                                               \
    return _stream;                                                       \
  }

#define OMNI_DECL_ENUM_STREAM_OPERATORS(ENUM)                             \
  inline QDataStream &operator>>(QDataStream & _stream, ENUM & _enum)     \
  {                                                                       \
    int _i = 0;                                                           \
    _stream >> _i;                                                        \
    _enum = omni::util::intToEnum<ENUM>(_enum);                           \
    return _stream;                                                       \
  }                                                                       \
  inline QDataStream& operator<<(QDataStream& _stream, ENUM const& _enum) \
  {                                                                       \
    _stream << omni::util::enumToInt(_enum);                              \
    return _stream;                                                       \
  }

#endif /* OMNI_SERIALIZATIONINTERFACE_H_ */

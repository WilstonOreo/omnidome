/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
 **/
#ifndef OMNI_SERIALIZATION_TRAITS_H_
#define OMNI_SERIALIZATION_TRAITS_H_

#include <type_traits>

#include <QString>
#include <QByteArray>
#include <omni/serialization/Interface.h>

namespace omni {
  namespace serialization {
    namespace traits {
      /// Traits for reading a value from stream
      template<typename T, bool BASE_OF_SERIALIZATION_INTERFACE =
                 std::is_base_of<serialization::Interface, T>::value>
      struct Read {
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          _stream >> _obj;
          return _stream;
        }
      };

      /// Handle classes that are derived from serialization::Interface
      template<typename T>
      struct Read<T, true>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          _obj.fromStream(_stream);
          return _stream;
        }
      };


      /// Template specialization for QByteArray
      template<>
      struct Read<QByteArray, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          uint32_t _length = 0;

          _stream >> _length;
          _obj.resize(_length);
          _stream.readRawData(_obj.data(), _length);
          return _stream;
        }
      };

      /// Template specialization for QString
      template<>
      struct Read<QString, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          QByteArray _bytes;

          Read<QByteArray>()(_stream, _bytes);
          _obj = QString::fromUtf8(_bytes);
          return _stream;
        }
      };

      template<typename T, bool BASE_OF_SERIALIZATION_INTERFACE =
                 std::is_base_of<serialization::Interface, T>::value>
      struct Write {
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          _stream << _obj;
          return _stream;
        }
      };

      template<typename T>
      struct Write<T, true>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          _obj.toStream(_stream);
          return _stream;
        }
      };

      /// Template specialization for QByteArray
      template<>
      struct Write<QByteArray, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          _stream << uint32_t(_obj.size());
          _stream.writeRawData(_obj.data(), _obj.size());
          return _stream;
        }
      };

      /// Template specialization for QString
      template<>
      struct Write<QString, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          Write<QByteArray>()(_stream, _obj.toUtf8());
          return _stream;
        }
      };
    }

    /**@brief Deserialize object of type T from stream
       @detail with optional additional arguments
     **/
    template<typename STREAM, typename T, typename ... ARGS>
    STREAM& deserialize(STREAM& _stream, T& _t, ARGS&& ... _args) {
        return traits::Read<T>(_args ...)(_stream, _t);
    }

    /// Deserialize object of type T and return its value
    template<typename T, typename STREAM>
    T deserializeReturn(STREAM& _stream, T const _default = T()) {
      T _t = _default;

      deserialize(_stream, _t);
      return _t;
    }

    /// Serialize object to stream
    template<typename STREAM, typename T>
    STREAM& serialize(STREAM& _stream, T const& _t) {
        return traits::Write<T>()(_stream, _t);
    }
  }

  using serialization::serialize;
  using serialization::deserializeReturn;
  using serialization::deserialize;
}

#endif /* OMNI_SERIALIZATION_TRAITS_H_ */

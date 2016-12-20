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
 */
#ifndef OMNI_SERIALIZATION_CONTAINER_H_
#define OMNI_SERIALIZATION_CONTAINER_H_

#include <array>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include "traits.h"

namespace omni {
  namespace serialization {
    namespace traits {
      /// Deserialize elements in array
      template<typename T, size_t N>
      struct Read<std::array<T, N>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          for (size_t i = 0; i < N; ++i) {
            _obj[i] = deserializeReturn<T>(_stream);
          }
        }
      };

      /// Deserialize elements in vector
      template<typename T, typename ALLOCATOR>
      struct Read<std::vector<T, ALLOCATOR>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          uint32_t _size = deserializeReturn<uint32_t>(_stream, 0);

          _obj.clear();

          for (uint32_t i = 0; i < _size; ++i) {
            _obj.push_back(deserializeReturn<T>(_stream));
          }
          return _stream;
        }
      };

      /// Deserialize elements in deque
      template<typename T, typename ALLOCATOR>
      struct Read<std::deque<T, ALLOCATOR>, false>:
        Read<std::vector<T, ALLOCATOR> >{};

      /// Handle classes that are derived from serialization::Interface
      template<typename T, typename ALLOCATOR>
      struct Read<std::list<T, ALLOCATOR>, false>:
        Read<std::vector<T, ALLOCATOR> >{};

      /// Deserialize elements in set
      template<typename T, typename SORT, typename ALLOCATOR>
      struct Read<std::set<T, SORT, ALLOCATOR>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          uint32_t _size = deserializeReturn<uint32_t>(_stream, 0);

          _obj.clear();

          for (uint32_t i = 0; i < _size; ++i) {
            _obj.insert(deserializeReturn<T>(_stream));
          }
          return _stream;
        }
      };

      /// Deserialize elements in map
      template<typename KEY, typename T, typename COMPARE, typename ALLOCATOR>
      struct Read<std::map<KEY, T, COMPARE, ALLOCATOR>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ& _obj) {
          uint32_t _size = deserializeReturn<uint32_t>(_stream, 0);

          _obj.clear();

          for (uint32_t i = 0; i < _size; ++i) {
            KEY && _key = deserializeReturn<KEY>(_stream);
            T && _value = deserializeReturn<T>(_stream);
            _obj[_key]  = _value;
          }
          return _stream;
        }
      };

      /// Serialize elements in array
      template<typename T, size_t N>
      struct Write<std::array<T, N>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          for (size_t i = 0; i < N; ++i) {
            serialize(_stream, _obj);
          }
          return _stream;
        }
      };

      /// Serialize elements in vector
      template<typename T, typename ALLOCATOR>
      struct Write<std::vector<T, ALLOCATOR>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          serialize(_stream, uint32_t(_obj.size()));
          for (auto& _element : _obj) {
            serialize(_stream, _element);
          }
          return _stream;
        }
      };

      /// Serialize elements in deque
      template<typename T, typename ALLOCATOR>
      struct Write<std::deque<T, ALLOCATOR>, false>:
        Write<std::vector<T, ALLOCATOR>, false>{};

      /// Serialize elements in list
      template<typename T, typename ALLOCATOR>
      struct Write<std::list<T, ALLOCATOR>, false>:
        Write<std::vector<T, ALLOCATOR>, false>{};

      /// Serialize elements in set
      template<typename T, typename SORT, typename ALLOCATOR>
      struct Write<std::set<T, SORT, ALLOCATOR>, false>:
        Write<std::vector<T, ALLOCATOR>, false>{};

      /// Serialize elements in map
      template<typename KEY, typename T, typename COMPARE, typename ALLOCATOR>
      struct Write<std::map<KEY, T, COMPARE, ALLOCATOR>, false>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _obj) {
          serialize(_stream, uint32_t(_obj.size()));

          for (auto& _keyValue : _obj) {
            serialize(_keyValue.first,  _obj);
            serialize(_keyValue.second, _obj);
          }
          return _stream;
        }
      };
    }
  }
}

#endif /* OMNI_SERIALIZATION_VECTOR_H_ */

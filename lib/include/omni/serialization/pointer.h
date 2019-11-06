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
#ifndef OMNI_SERIALIZATION_POINTER_H_
#define OMNI_SERIALIZATION_POINTER_H_

#include <memory>
#include <functional>
#include <omni/Factory.h>
#include <omni/serialization/traits.h>

namespace omni {
  namespace serialization {
    namespace traits {
      struct ReadPtr {
        template<typename STREAM, typename F>
        STREAM& operator()(STREAM& _stream, F _f) {
          auto _id = deserializeReturn<Id>(_stream, Id("none"));

          if (_id == "none") return _stream;

          auto *_obj = _f(_id);

          if (_obj)
          {
            _obj->fromStream(_stream);
          }
          else
          {
            throw omni::exception::Serialization(_id);
          }
          return _stream;
        }
      };

      template<typename T, bool BASE_OF_SERIALIZATION_INTERFACE>
      struct Write<T *, BASE_OF_SERIALIZATION_INTERFACE>{
        template<typename STREAM, typename OBJ>
        STREAM& operator()(STREAM& _stream, OBJ const& _t) {
          if (!_t)
          {
            serialize(_stream, Id("none"));
            return _stream;
          }
          serialize(_stream, _t->getTypeId());
          _t->toStream(_stream);
          return _stream;
        }
      };

      template<typename T, typename DELETER, bool BASE_OF_SERIALIZATION_INTERFACE>
      struct Write<std::unique_ptr<T, DELETER>, BASE_OF_SERIALIZATION_INTERFACE>:
        Write<T *, BASE_OF_SERIALIZATION_INTERFACE>{};
    }

    /**@brief Deserialize a pointer from stream.
       @detail Functor f must return a pointer which is constructed
               from a factory with an id.
               If pointer is not null, it will be deserialized from stream.
               Otherwise, a serialization exception is thrown.
     **/
    template<typename STREAM, typename F, typename ... ARGS>
    STREAM& deserializePtr(STREAM& _stream,
                           F _f,
                           ARGS&& ... _args) {
      return traits::ReadPtr(_args ...)(_stream, _f);
    }
  }

  using serialization::deserializePtr;
}

#endif /* OMNI_SERIALIZATION_POINTER_H_ */

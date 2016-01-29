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

#ifndef BOOSTX_TYPE_INFO_HPP_
#define BOOSTX_TYPE_INFO_HPP_

#include <string>

/// Convenience macro for declaring a type info struct

#define BOOSTX_DECL_TYPE_INFO(NAME,TYPEID,STATIC_MEMBER,VIRTUAL_MEMBER)\
  template<typename T>\
  struct NAME\
  {\
    typedef TYPEID typeid_type;\
    typeid_type operator()() const\
    {\
      return T::STATIC_MEMBER();\
    }\
    template<typename PTR>\
    typeid_type operator()(PTR&& _ptr) const\
    {\
      return _ptr->VIRTUAL_MEMBER();\
    }\
  };

/// Convenience macro for defining a type id for a single class
#define BOOSTX_TYPE_ID(TYPEID_TYPE,TYPEID,STATIC_MEMBER,VIRTUAL_MEMBER)\
    inline static TYPEID_TYPE STATIC_MEMBER() { return TYPEID; }\
    inline virtual TYPEID_TYPE VIRTUAL_MEMBER() const { return STATIC_MEMBER(); }

namespace boostx
{
  /// Declare default type id mechanism
  BOOSTX_DECL_TYPE_INFO(
      type_info, // Name of type id struct
      std::string, // Type id type
      type_id, // Static member function
      get_type_id // Virtual member function of abstract interface
  )
}

#define BOOSTX_DECL_TYPE_ID_DEFAULT(TYPEID)\
  BOOSTX_TYPE_ID(std::string,TYPEID,type_id,get_type_id)


#endif /* BOOSTX_TYPE_INFO_HPP_ */

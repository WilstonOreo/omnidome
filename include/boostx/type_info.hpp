//          Copyright Wilston Oreo 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

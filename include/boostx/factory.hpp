//                Copyright Wilston Oreo 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOSTX_FACTORY_HPP_
#define BOOSTX_FACTORY_HPP_

#include <unordered_map>
#include <functional>
#include <type_traits>

namespace boostx
{
  template<typename T, typename REGISTRY, typename...ARGS>
  struct registrar;

  /**@brief The central factory class.
   * @tparam INTERFACE Typename of the abstract interface
   * @tparam TYPEINFO Template template parameter for the typeinfo
   * @tparam ...ARGS Template parameter which comply to the constructor signature of the interface 
   */
  template<typename INTERFACE, template<class> class TYPEINFO, typename...ARGS>
  struct factory
  {
    /// Typedef for our abstract interface
    typedef INTERFACE interface_type;

    /// Typedef for this factory type
    typedef factory<interface_type,TYPEINFO,ARGS...> type;
    
    /// Template alias for typeinfo type
    template<typename T>
    using typeinfo_type = TYPEINFO<T>;
    
    /// Get key type from typeinfo struct 
    typedef typename typeinfo_type<interface_type>::typeid_type key_type;

    /// Constructor function type
    typedef std::function<interface_type*(ARGS...)> constructor_type;
    
    /// Internal type of map for storing ids
    typedef std::unordered_map<key_type,constructor_type> class_map_type;

    /// Alias template for registar
    template<typename T>
    using registrar_type = registrar<T,type,ARGS...>;

    /// Checks if class with element id is registered
    static bool exists(const key_type& _key)
    {
      return private_classes().count(_key) > 0;
    }

    /// Registers class with custom key
    template<typename T>
    static void reg(const key_type& _key)
    {
      if (exists(_key)) return;
      private_classes()[_key] = registrar_type<T>::create;
    }
    
    /// Registers class by getting key from registrar's type id
    template<typename T>
    static void reg()
    {
      key_type_check<T>();
      reg<T>(registrar_type<T>::type_id());
    }
    
    /// Unregisters class with custom key 
    static void unreg(const key_type& _key)
    {
      if (!exists(_key)) return;  
      private_classes().erase(_key);
    }

    /// Unregisters class by getting key from registrar
    template<typename T>
    static void unreg()
    {
      key_type_check<T>();
      unreg(registrar_type<T>::type_id());
    }

    /// Instantiates an object of the class by id and constructor arguments
    static interface_type* create(const key_type& _key, const ARGS&..._args)
    {
      if (!exists(_key)) return nullptr;
      return private_classes()[_key](_args...);
    }

    /// Gives readonly access to registered classes
    static class_map_type const& classes()
    {
      return private_classes();
    }

  private:
    /// Checks if key types are the same
    template<typename T>
    static void key_type_check()
    {
      // Check if T is a base class of interface
      static_assert(std::is_base_of<interface_type,T>::value,
          "Given type T must be a base class of the interface class"); 

      // Check if registrar's type id and key type are the same
      typedef std::is_same<decltype(registrar_type<T>::type_id()),key_type> is_same_type;
      static_assert(is_same_type::value,
          "Type of type id to be registered and registrys type id must be equal!");
    }

    static class_map_type& private_classes()
    {
      static class_map_type _classes;
      return _classes;
    }
  };

  /**@brief Registrar mixin class which holds flag of registered class, to be derived from T
   * @tparam T type of derived class
   * @tparam REGISTRY type of registry the registrar class uses
   * @tparam ...ARGS Template parameter which comply to the constructor signature of the interface 
   **/
  template<typename T, typename REGISTRY, typename...ARGS>
  struct registrar
  {
    /// Typedef for registry
    typedef REGISTRY registry_type;

    /// Gets key type from registry
    typedef typename registry_type::key_type key_type;

    /// Gets interface type from registry
    typedef typename registry_type::interface_type interface_type;
    
    /// Get type info type from registry
    typedef typename registry_type::template typeinfo_type<T> typeinfo_type;
 
    /// Friend of registry so registry can access registrar's private functions
    friend registry_type;
   
    /// This static instance is responsible for auto registration
    static registrar* instance_;

    /// Constructor of registrar registeres the object if not already registered
    registrar()
    { 
      if (!registered()) registry_type::template reg<T>(type_id());
    }

  private:
    /// Overload assignment operator so no copying is possible
    registrar& operator=(const registrar&)
    {
      return *this;
    }

    /// The class is registered when the instance is not a null pointer
    static bool registered()
    {
      return instance_ != nullptr;
    }

    /// Create an instance of the interface and return raw pointer
    static interface_type* create(const ARGS&..._args)
    {
      return new T(_args...);
    }

    /// Return type id by calling the call operator of type info template 
    static decltype(typeinfo_type()()) type_id()
    {
      return typeinfo_type()();
    }
 
    /// Holds the instance which cares about auto registration
    static registrar* instance()
    {
      static registrar theInstance;
      instance_ = &theInstance;
      return instance_;
    }
  };

  /// Our static instance
  template<typename T, typename R, typename...ARGS>
  registrar<T,R,ARGS...>* registrar<T,R,ARGS...>::instance_
    = registrar<T,R,ARGS...>::instance();
}


#endif /* BOOSTX_FACTORY_HPP_ */


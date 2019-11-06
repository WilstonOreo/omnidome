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

#ifndef OMNI_INPUT_LIST_H
#define OMNI_INPUT_LIST_H

#include <memory>
#include <vector>
#include <omni/Factory.h>
#include <omni/input/Interface.h>
#include <omni/serialization/PropertyMapSerializer.h>


namespace omni {
  namespace input {

    /**@brief Input List contains a list of inputs, accessible over an QString id
     *@detail
     Input List is the root input of a session's input tree.
     Input List is serializable via QDataStream.
     **/
    class OMNI_EXPORT List :
      public PropertyMapSerializer,
      private std::vector< std::unique_ptr< Input > >
    {
      public:  
        using container_type = std::vector< std::unique_ptr< Input >> ; 
        List() {}
        ~List() {}

        using container_type::size;
        using container_type::empty;
        using container_type::begin;
        using container_type::end;

        /**@brief Add new input with given type id. Returns nullptr if input
           with typeid does not exist
           @detail Id is automatically generated
         *@param _typeId Type id of input to determine which kind of input is
         *created
         *@return Pair with input id and pointer to added input
         **/
        Input* addInput(Id const& _typeId);

        /// Remove input and reset current index if necessary
        void                       removeInput(int);

        /// Return input at index
        Input       * operator[](int index) const;
    
        Input*        getInput(int index) const;

        /// Delete all inputs
        void          clear();

        Input* current() const;


        /// Input with id and return pointer to input when successfully added
        Input* addInput(Input *_i);

        /// Return ID of current input
        int           currentIndex() const;

        /// Set current input by ID
        void          setCurrentIndex(int);

        /// Deserialize from property map
        void          fromPropertyMap(PropertyMap const&);

        /// Serialize to property map
        void          toPropertyMap(PropertyMap&) const;

        /// Test for equality
        friend bool   operator==(List const&,
                                 List const&);

      private:
        bool validIndex(int) const; 

        int currentIndex_ = -1;
    };
  }

  typedef input::List InputList;
}

OMNI_DECL_STREAM_OPERATORS(omni::input::List)


#endif /* OMNI_INPUT_LIST_H */

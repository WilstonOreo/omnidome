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

#ifndef OMNI_INPUT_LIST_H
#define OMNI_INPUT_LIST_H

#include <memory>
#include <map>
#include <omni/Id.h>
#include <omni/input/Interface.h>

namespace omni
{
  namespace input
  {
    /**@brief Input List contains a list of inputs. A TestImage input is always present at index 0
      *@detail Input List is serializable via QDataStream
     **/
    class List : private std::vector<std::unique_ptr<Input>>
    {
    public: 
      typedef std::vector<std::unique_ptr<Input>> container_type;
      
      using container_type::size;
      using container_type::empty;
      using container_type::begin;
      using container_type::end;

      List();

      /**@brief Add new input with given type id. Returns nullptr if input with typeid does not exist 
        *@param _typeId Type id of input to determine which kind of input is created
       **/
      Input* add(Id const& _typeId);
 
      /**@brief Add new input with id and type id. Returns nullptr if mapping already exists 
         @tparam TYPE Type of input
         @tparam ...ARGS Type of arguments passed to constructor of input of TYPE
        *@param _id Id for input
        *@param ..._args Arguments passed to constructor 
       **/
      template<typename TYPE, typename...ARGS>
      TYPE* add(ARGS&&..._args)
      {  
        container_type::emplace_back(new TYPE(_args...));
        return static_cast<TYPE*>(container_type::back().get());
      }

      /// Return input at index 
      Input* operator[](int);
      
      /// Returns input at index (const version)
      Input const* operator[](int) const;

      /// Removes input at index
      void remove(int);

      /// Delete all inputs
      void clear();
      
      /// Deserialize from stream
      void fromStream(QDataStream&);

      /// Serialize to stream
      void toStream(QDataStream&) const;

      /**@brief Returns pointer to current input
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input* current();
      
      /**@brief Returns pointer to current input (const version)
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input const* current() const;
      
      /// Return ID of current input
      int currentIndex() const; 

      /// Set current input by ID
      void setCurrentIndex(int);

      /// Test for equality
      friend bool operator==(List const&,List const&);

    private:
      bool validIndex(int) const;

      int currentIndex_ = 0;
    };
  }

  typedef input::List InputList;
}

QDataStream& operator<<(QDataStream&, omni::input::List const&);
QDataStream& operator>>(QDataStream&, omni::input::List&);


#endif /* OMNI_INPUT_LIST_H */

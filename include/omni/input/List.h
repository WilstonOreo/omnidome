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

#include <algorithm>
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
    class List : private std::map<QString,std::unique_ptr<Input>>
    {
    public:
      typedef std::map<QString,std::unique_ptr<Input>> container_type;

      using container_type::size;
      using container_type::empty;
      using container_type::begin;
      using container_type::end;

      List();

      /**@brief Add new input with given type id. Returns nullptr if input with typeid does not exist
         @param _id Id for the input
        *@param _typeId Type id of input to determine which kind of input is created
       **/
      Input* add(QString const& _id, Id const& _typeId);

      /**@brief Add new input with given type id. Returns nullptr if input with typeid does not exist
         @detail Id is automatically generated
        *@param _typeId Type id of input to determine which kind of input is created
        *@return Pair with input id and pointer to added input
       **/
      std::pair<QString,Input*> add(Id const& _typeId);

      /// Return input at index
      Input* operator[](QString const& _id);

      /// Returns input at index (const version)
      Input const* operator[](QString const& _id) const;

      /// Removes input at index
      void remove(QString const& _id);

      /// Delete all inputs
      void clear();

      /**@brief Returns pointer to current input
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input* current();

      /**@brief Returns pointer to current input (const version)
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input const* current() const;

      /// Return ID of current input
      QString currentId() const;

      /// Set current input by ID
      void setCurrentId(QString const&);

      /// Deserialize from stream
      void fromStream(QDataStream&);

      /// Serialize to stream
      void toStream(QDataStream&) const;

      /// Test for equality
      friend bool operator==(List const&,List const&);

    private:
      QString generateId() const;

      QString currentId_;
    };
  }

  typedef input::List InputList;
}

OMNI_DECL_STREAM_OPERATORS(omni::input::List)



#endif /* OMNI_INPUT_LIST_H */

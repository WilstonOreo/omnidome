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

#include <algorithm>
#include <memory>
#include <map>
#include <omni/Id.h>
#include <omni/input/Interface.h>
#include <omni/input/Controller.h>


namespace omni {
  namespace input {
    class Controller;

    /**@brief Input List contains a list of inputs, accessible over an QString id
     *@detail
     Input List is the root input of a session's input tree.
     Input List is serializable via QDataStream.
     **/
    class List : public input::Interface {
      public:
        List(Interface const * = nullptr);

        /// A list cannot be registered in factory
        inline void registerInFactory() const {
        }

        /// Type id is "List"
        inline Id getTypeId() const {
          return Id("List");
        }

        /**@brief Add new input with given type id. Returns nullptr if input
           with typeid does not exist
           @detail Id is automatically generated
         *@param _typeId Type id of input to determine which kind of input is
         *created
         *@return Pair with input id and pointer to added input
         **/
        std::pair<QString, Input *>addInput(Id const& _typeId);

        /// Remove input and reset current index if necessary
        void                       removeInput(QString const& _id);

        /// List can have children
        inline bool canHaveChildren() const {
          return true;
        }

        /// List has no size
        inline QSize size() const {
          return QSize(0, 0);
        }

        /// List does not have a texture
        inline GLuint textureId() const {
          return 0;
        }

        inline QWidget* widget() {
          return nullptr;
        }

        /// Return input at index
        Input       * operator[](QString const& _id);

        /// Returns input at index (const version)
        Input const * operator[](QString const& _id) const;

        /// Delete all inputs
        void          clear();

        /**@brief Returns pointer to current input
         * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
         **/
        Input       * current();

        /**@brief Returns pointer to current input (const version)
         * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
         **/
        Input const * current() const;

        /// Return ID of current input
        QString       currentId() const;

        /// Set current input by ID
        void          setCurrentId(QString const&);


        /// Return pointer to controller
        Controller*         controller();

        /// Return pointer to controller (const version)
        Controller const*   controller() const;

        /// Deserialize from property map
        void          fromPropertyMap(PropertyMap const&);

        /// Serialize to property map
        void          toPropertyMap(PropertyMap&) const;

        /// Test for equality
        friend bool   operator==(List const&,
                                 List const&);

      private:
        /// Generate a new id for input
        QString generateId() const;

        QString currentId_;
    };
  }

  typedef input::List InputList;
}

OMNI_DECL_STREAM_OPERATORS(omni::input::List)


#endif /* OMNI_INPUT_LIST_H */

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
#ifndef OMNI_PATCH_LIST_H_
#define OMNI_PATCH_LIST_H_

#include <map>
#include <memory>
#include <QString>
#include <omni/patch/Interface.h>

namespace omni {
  namespace patch {
    class List : private std::map<QString,std::unique_ptr<Patch>>   {
    public:
        typedef std::map<QString,std::unique_ptr<Patch>> container_type;

        using container_type::empty;
        using container_type::size;
        using container_type::begin;
        using container_type::end;

        /**@brief Add new patch with given type id. Returns nullptr if input
           with typeid does not exist
           @param _id Id for the patch
         *@param _typeId Type id of patch to determine which kind of input is
         * created
         **/
        Patch* addPatch(QString const& _id,
                        Id const& _typeId);

        /// Input with id and return pointer to input when successfully added
        Patch* addPatch(QString const& _id,
                        Patch*);

        /**@brief Add new input with given type id. Returns nullptr if input
           with typeid does not exist
           @detail Id is automatically generated
         *@param _typeId Type id of input to determine which kind of input is
         *created
         *@return Pair with input id and pointer to added input
         **/
        std::pair<QString, Patch *> addPatch(Id const& _typeId);

        /// Return pointer of input with id, nullptr if input does not exist
        Patch           * getPatch(QString const& _id);

        /// Return pointer of input with id, nullptr if input does not exist
        Patch const     * getPatch(QString const& _id) const;


        void removePatch(QString const& _id);

        /// Delete all patches
        void          clear();

        /// Return ID of current patch
        QString       currentId() const;

        /// Set current patch by id and deselected all other patches
        void          setCurrentId(QString const&);

        /// Return patch at index
        Patch       * operator[](QString const& _id) const;

        /**@brief Returns pointer to current patch
         * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
         **/
        Patch       * current();

        /// Select / deselect patch
        void selectPatch(QString const& _id,
                         bool _select = true);

        /// Return flag whether patch with id is selected
        bool                        patchIsSelected(QString const&) const;

    private:
        /// Generate a new id for input
        QString generateId() const;

        /// Ids of selected patches
        std::set<QString> selectedPatches_;
    };
  }

  typedef patch::List PatchList;
}

#endif /* OMNI_PATCH_LIST_H_ */

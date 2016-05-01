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

#include <omni/patch/List.h>

#include <omni/serialization/container.h>

namespace omni {
  namespace patch {
    Patch * List::addPatch(QString const& _id, Id const& _typeId) {
      std::unique_ptr<Patch> _patch(patch::Factory::create(_typeId));

      if (!_patch) return nullptr;

      container_type::operator[](_id) = std::move(_patch);
      return container_type::at(_id).get();
    }

    std::pair<QString, Patch *>List::addPatch(Id const& _typeId) {
      auto _id = generateId();

      return std::make_pair(_id, addPatch(_id, _typeId));
    }

    Patch * List::addPatch(QString const& _id, Patch *_patch) {
      if (container_type::count(_id) != 0) {
        return nullptr;
      }
             container_type::operator[](_id).reset(_patch);
      return container_type::operator[](_id).get();
    }

    /// Return pointer of input with id, nullptr if input does not exist
    Patch           * List::getPatch(QString const& _id) {
      if (!container_type::count(_id)) return nullptr;

      return container_type::at(_id).get();
    }

    /// Return pointer of input with id, nullptr if input does not exist
    Patch const     * List::getPatch(QString const& _id) const {
      if (!container_type::count(_id)) return nullptr;

      return container_type::at(_id).get();
    }

    /// Delete all patches
    void          List::clear() {
      container_type::clear();

      selectedPatches_.clear();
    }

    void List::removePatch(QString const& _id) {
      container_type::erase(_id);

      selectedPatches_.erase(_id);
    }

    /// Return ID of current input
    QString       List::currentId() const {
      return selectedPatches_.empty() ? QString("") : *selectedPatches_.begin();
    }

    /// Select / deselect patch
    void List::selectPatch(QString const& _id, bool _select) {
      auto *_patch = getPatch(_id);

      if (!_patch) return;

      if (_select) {
        selectedPatches_.insert(_id);
      } else {
        selectedPatches_.erase(_id);
      }
    }

    /// Return flag whether patch with id is selected
    bool List::patchIsSelected(QString const& _id) const {
      return selectedPatches_.count(_id) > 0;
    }

    Patch * List::current() {
      return getPatch(currentId());
    }

    /// Set current input by ID
    void          List::setCurrentId(QString const& _currentId) {
      selectedPatches_.clear();

      if (getPatch(_currentId) == nullptr) {
        return;
      }
      selectedPatches_.insert(_currentId);
    }

    /// Deserialize from stream
    void List::fromStream(QDataStream& _is) {
      int _size = 0;
      deserialize(_is, _size);

      for (int i = 0; i < _size; ++i) {
          auto _id = deserializeReturn<QString>(_is);
          deserializePtr(_is, [&](Id const& _typeId) ->
                         patch::Interface *
          {
            return addPatch(_id, _typeId);
          });
      }
      deserialize(_is,selectedPatches_);
    }

    /// Serialize to stream
    void List::toStream(QDataStream& _os) const {
      serialize(_os, int(size()));

      for (auto& _idPatch : (*this)) {
          auto& _id    = _idPatch.first;
          auto& _patch = _idPatch.second;
          serialize(_os, _id);
          serialize(_os, _patch.get());
      }

      serialize(_os,selectedPatches_);
    }

    QString List::generateId() const {
      QString _id("0");

      for (size_t i = 0; i <= size(); ++i) {
        _id = QString("%1").arg(i);

        if (getPatch(_id) == nullptr)
        {
          return _id;
        }
      }
      return _id;
    }
  }
}

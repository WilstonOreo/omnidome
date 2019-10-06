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
 */

#include "SyphonServerItemModel.h"

namespace omni {
  namespace ui {
    namespace input {
      SyphonServerItemModel::SyphonServerItemModel(QObject* _parent) :
        QStandardItemModel(_parent) {
          setColumnCount(3);
      }

      SyphonServerItemModel::~SyphonServerItemModel() {


      }

      omni::input::SyphonServerDescription SyphonServerItemModel::getDescription(int _index) const {
        if (_index < 0 || _index >= rowCount())
          return omni::input::SyphonServerDescription();

        auto* i = item(_index);
        return omni::input::SyphonServerDescription(
          i->data(AppNameRole).toString(),
          i->data(ServerNameRole).toString()
        );
      }

      void SyphonServerItemModel::addDescription(
        omni::input::SyphonServerDescription const& _desc) {
        QStandardItem* _labelItem = new QStandardItem(
            _desc.applicationName() + " : " +
            _desc.serverName());
        _labelItem->setEditable(false);
        _labelItem->setData(_desc.applicationName(),AppNameRole);
        _labelItem->setData(_desc.serverName(),ServerNameRole);
        invisibleRootItem()->appendRow(_labelItem);
      }
    }
  }
}

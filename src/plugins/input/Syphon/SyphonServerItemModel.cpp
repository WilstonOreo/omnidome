/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

        auto _appNameItem = item(_index,1);
        auto _serverNameItem = item(_index,2);

        if (!_appNameItem || !_serverNameItem)
          return omni::input::SyphonServerDescription();

        return omni::input::SyphonServerDescription(
          _appNameItem->text(),
          _serverNameItem->text()
        );
      }

      void SyphonServerItemModel::addDescription(
        omni::input::SyphonServerDescription const& _desc) {
        QList<QStandardItem*> _row;
        QStandardItem* _labelItem = new QStandardItem(
            _desc.applicationName() + " : " +
            _desc.serverName());
        _labelItem->setEditable(false);
        QStandardItem* _appNameItem = new QStandardItem(_desc.applicationName());
        _appNameItem->setEditable(false);
        QStandardItem* _serverNameItem = new QStandardItem(_desc.serverName());
        _serverNameItem->setEditable(false);
        _row << _labelItem;
        _row << _appNameItem;
        _row << _serverNameItem;
        invisibleRootItem()->appendRow(_row);
      }
    }
  }
}

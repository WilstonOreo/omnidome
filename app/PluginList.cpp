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

#include "PluginList.h"
#include <omni/PluginLoader.h>

namespace omni {
  namespace ui {
    PluginList::PluginList(QWidget *_parent) :
      QTreeView(_parent) {
      prepareModel();

      for (auto& _pluginInfo : PluginLoader::loadedPlugins()) {
        addItem(_pluginInfo);
      }

      for (int i = 0; i < model_->columnCount(); ++i) {
        resizeColumnToContents(i);
      }
    }

    PluginList::~PluginList() {}

    void PluginList::prepareModel() {
      model_.reset(new QStandardItemModel());
      model_->clear();
      model_->setColumnCount(8);
      model_->setHeaderData(0, Qt::Horizontal, "Plugin Name");
      model_->setHeaderData(1, Qt::Horizontal, "Type");
      model_->setHeaderData(2, Qt::Horizontal, "Class name");
      model_->setHeaderData(3, Qt::Horizontal, "Author");
      model_->setHeaderData(4, Qt::Horizontal, "URL");
      model_->setHeaderData(5, Qt::Horizontal, "Version");
      model_->setHeaderData(6, Qt::Horizontal, "Description");
      model_->setHeaderData(7, Qt::Horizontal, "File");
      setModel(model_.get());
    }

    void PluginList::addItem(PluginInfo const& _info) {
      QList<QStandardItem *> _row;
      auto _makeItem = [](QString const& _t) -> QStandardItem * {
                         QStandardItem *_item = new QStandardItem(_t);

                         _item->setEditable(false);
                         return _item;
                       };

      _row << _makeItem(_info.name());
      _row << _makeItem(_info.type());
      _row << _makeItem(_info.className());
      _row << _makeItem(_info.author());
      _row << _makeItem(_info.url());
      _row << _makeItem(_info.versionString());
      _row << _makeItem(_info.description());
      _row << _makeItem(_info.file());

      model_->invisibleRootItem()->appendRow(_row);
    }
  }
}

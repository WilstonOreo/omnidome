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

#ifndef OMNI_UI_PLUGINLIST_H_
#define OMNI_UI_PLUGINLIST_H_

#include <memory>
#include <QTreeView>
#include <QStandardItemModel>
#include <omni/PluginInfo.h>

namespace omni {
  namespace ui {
    /// List view to show all loaded plugins
    class PluginList : public QTreeView {
      Q_OBJECT

      public:
        PluginList(QWidget * = nullptr);
        ~PluginList();

      private:
        void prepareModel();
        void addItem(PluginInfo const&);
        std::unique_ptr<QStandardItemModel> model_;
    };
  }
}

#endif /* OMNI_UI_PLUGINLIST_H_ */

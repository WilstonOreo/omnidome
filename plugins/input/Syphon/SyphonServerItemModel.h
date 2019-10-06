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
#ifndef OMNI_UI_INPUT_SYPHONSERVERITEMMODEL_H_
#define OMNI_UI_INPUT_SYPHONSERVERITEMMODEL_H_

#include <QStandardItemModel>
#include "SyphonServerDescription.h"

namespace omni {
  namespace ui {
    namespace input {
      class SyphonServerItemModel : public QStandardItemModel {
        Q_OBJECT
      public:
        enum ItemRole {
          AppNameRole = Qt::UserRole + 1,
          ServerNameRole
        };

        SyphonServerItemModel(QObject* _parent = nullptr);
        ~SyphonServerItemModel();

        omni::input::SyphonServerDescription getDescription(int _index) const;
        void addDescription(omni::input::SyphonServerDescription const&);
      };
    }
  }
}

#endif /* OMNI_UI_INPUT_SYPHONSERVERITEMMODEL_H_ */

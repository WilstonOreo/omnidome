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

#ifndef OMNI_UI_INPUT_H_
#define OMNI_UI_INPUT_H_

#include <memory>
#include "DockWidget.h"
#include <QAction>
#include <QStandardItemModel>
#include <omni/input/Interface.h>
#include "mixin/DataModel.h"
#include "mixin/ParameterWidget.h"

namespace omni {
  class Session;

  namespace ui {
    namespace Ui {
      class Input;
    }

    /// Input List widget
    class Input :
      public DockWidget,
      public mixin::SharedDataModel<Input,Session>,
      private mixin::ParameterWidget<> {
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(Input,Session)

      public:
        Input(QWidget * = nullptr);
        ~Input();

      public slots:
        void addInput(QAction *);

        /// Removes all selected inputs
        void removeSelection();

        /// Removes all inputs
        void clear();

        /// Select input at model index
        void changeSelection(QModelIndex);

      signals:
        void inputChanged();
        void inputRemoved();
        void dataModelChanged();
        void inputIndexChanged();

      private:
        /// Update sliders from given blend mask
        void dataToFrontend() override;

        /// Assign slider values to blend mask
        inline bool frontendToData() override {
          return false;
        }

        void setupInputMenu(QMenu*);

        void    inputUpdatedEmitter();

        void    selectInput(int);

        /// Setup the item model with columns etc
        void    prepareModel();

        bool    showSettingsDialog(input::Interface *);

        /// Add an item to input list widget
        void    addItem(input::Interface*);

        void    parameterWidgetSetupOptions(QWidget *_paramWidget) const override;

        void    showParameterWidget();

        std::unique_ptr<Ui::Input> ui_;
        std::unique_ptr<QStandardItemModel> model_;
    };
  }
}


#endif /* OMNI_UI_INPUT_H_ */

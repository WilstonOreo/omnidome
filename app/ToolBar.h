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
#ifndef OMNI_UI_TOOLBAR_H_
#define OMNI_UI_TOOLBAR_H_

#include <QToolBar>
#include <omni/Session.h>
#include "mixin/DataModel.h"

class QToolButton;

namespace omni {
  namespace ui {
    /// Omnidome main window toolbar
    class ToolBar :
      public QToolBar,
      public mixin::SharedDataModel<ToolBar,Session>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(ToolBar,Session)

      public:
        ToolBar(QWidget * = nullptr);
        ~ToolBar() override;

      public slots:
        void setScreenSetupMode();
        void setArrangeMode();
        void setWarpMode();
        void setBlendMode();
        void setColorCorrectionMode();
        void setExportMode();
        void setLiveMode();

        /// Show About dialog
        void showAbout();

        /// Set button states (e.g. disable buttons when modes are not accessible)
        void buttonStates();

      signals:
        void dataModelChanged();

      private:
        void setMode(Session::Mode);

        /// Update buttons from session
        void dataToFrontend() override;

        /// Set session mode from buttons
        bool frontendToData() override {
          return false;
        }

        QToolButton *btnSettings_        = nullptr;
        QToolButton *btnScreenSetup_     = nullptr;
        QToolButton *btnArrange_         = nullptr;
        QToolButton *btnWarp_            = nullptr;
        QToolButton *btnBlend_           = nullptr;
        QToolButton *btnColorCorrection_ = nullptr;
        QToolButton *btnExport_          = nullptr;
        QToolButton *btnLive_            = nullptr;
    };
  }
}

#endif /* OMNI_UI_TOOLBAR_H_ */

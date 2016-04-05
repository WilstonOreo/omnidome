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

#ifndef OMNI_UI_CANVAS_H_
#define OMNI_UI_CANVAS_H_

#include <memory>
#include <omni/visual/ProjectorViewMode.h>
#include <omni/ui/mixin/DataModel.h>
#include <omni/ui/mixin/ParameterWidget.h>
#include <omni/ui/CanvasParameters.h>
#include <omni/TypeIdMemory.h>
#include "DockWidget.h"

namespace omni {
  class Session;

  namespace ui {
    namespace Ui {
      class Canvas;
    }

    /// Dockwidget for editing canvas type and parameters
    class Canvas :
      public DockWidget,
      public mixin::SharedDataModel<Session>,
      private mixin::ParameterWidget {
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(Session)
      public:
        Canvas(QWidget * = nullptr);
        ~Canvas();

      signals:
        void dataModelChanged();
        void canvasTypeChanged();
        void sceneSizeChanged();

      public slots:
        /// Select canvas type with id
        void selectCanvasType(QString);

        /// Set size of scene
        void setSceneSize(float);

      private:
        /// Update widgets from current mapping
        void dataToFrontend();

        /// Assign widget values to current mapping
        bool frontendToData();

        void showParameterWidget();

        std::unique_ptr<Ui::Canvas> ui_;

        TypeIdMemory<canvas::Interface> canvasMemory_;
    };
  }
}

#endif /* OMNI_UI_CANVAS_H_ */

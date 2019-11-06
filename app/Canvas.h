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

#ifndef OMNI_UI_CANVAS_H_
#define OMNI_UI_CANVAS_H_

#include <memory>
#include <omni/visual/ProjectorViewMode.h>
#include "mixin/DataModel.h"
#include "mixin/ParameterWidget.h"
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
      public mixin::SharedDataModel<Canvas,Session>,
      private mixin::ParameterWidget<ui::CanvasParameters> {
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(Canvas,Session)
      public:
        Canvas(QWidget * = nullptr);
        ~Canvas() override;

      signals:
        void dataModelChanged();
        void canvasTypeChanged();

      public slots:
        /// Select canvas type with id
        void selectCanvasType(QByteArray const&);

        /// Set size of scene for canvas parameter widget
        void updateSceneSize(bool);

        /// Set unit suffix string for canvas parameter widget
        void updateUnits();

      private:
        /// Update widgets from current mapping
        void dataToFrontend() override;

        /// Assign widget values to current mapping
        bool frontendToData() override;

        void showParameterWidget();

        std::unique_ptr<Ui::Canvas> ui_;

        TypeIdMemory<canvas::Interface> canvasMemory_;
    };
  }
}

#endif /* OMNI_UI_CANVAS_H_ */

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

#include "Canvas.h"

#include "ui_omni_ui_Canvas.h"

#include <QVBoxLayout>
#include <QVariant>

#include <omni/Session.h>
#include <omni/canvas/Interface.h>
#include <omni/ui/ExceptionList.h>

namespace omni {
    namespace ui {
        Canvas::Canvas(QWidget *_parent) :
            DockWidget(ui_,_parent)
        {
            this->setup(ui_);

            // Add canvas types from Factory
            for (auto& _idCanvasClass : omni::canvas::Interface::factory().classes())
            {
                auto& _id = _idCanvasClass.first;
                ui_->boxCanvasSelect->addItem(QIcon(QString(":/canvas/") + _id +
                                                    QString(
                                                        ".png")), _id,
                                              QVariant(_id));
            }

            connect(ui_->boxCanvasSelect, SIGNAL(currentIndexChanged(QString)), this,
                    SLOT(selectCanvasType(QString)));
        }

        Canvas::~Canvas()
        {}

        void Canvas::dataToFrontend()
        {
            if (!dataModel()->canvas())
            {
                dataModel()->setCanvas("HalfDome");
                selectCanvasType("HalfDome");
            }
            // Search combobox for available canvas types
            int _index = 0;

            for (int i = 0; i < ui_->boxCanvasSelect->count(); ++i)
            {
                QString _id = ui_->boxCanvasSelect->itemData(i).toString();

                if (_id == dataModel()->canvas()->getTypeId())
                {
                    _index = i;
                }
            }

            ui_->boxCanvasSelect->setCurrentIndex(_index);
            showParameterWidget();
        }

        bool Canvas::frontendToData() {
            return false;
        }

        void Canvas::updateSceneSize(bool _rescaleValues) {
          auto* _paramWidget = this->parameterWidget();

          if (_paramWidget && dataModel()) {
            _paramWidget->setRescaleValues(_rescaleValues);
            _paramWidget->setScale(dataModel()->scene().size());
          }
        }

        void Canvas::updateUnits() {
          auto* _paramWidget = this->parameterWidget();

          if (_paramWidget && dataModel()) {
            /// Update parameter slider unit suffixes
            _paramWidget->setUnit(dataModel()->scene().unit().abbreviation());
          }
        }

        void Canvas::selectCanvasType(QByteArray const& _id)
        {
            if (!dataModel() || signalsBlocked()) return;

            tryWithExceptionList<Exception>([&]() {
              canvasMemory_.store(dataModel()->canvas());

              /// Dont do anything if type id has not changed
              if (dataModel()->canvas()->getTypeId() == _id) {
                return;
              }

              dataModel()->setCanvas(_id);
              canvasMemory_.restore(dataModel()->canvas());
              showParameterWidget();

              emit canvasTypeChanged();
            });
        }

        void Canvas::showParameterWidget() {
            if (this->setupParameterWidget(widget(),dataModel()->canvas())) {
                this->parameterWidget()->setDataModel(dataModel().get());
                dataModel()->canvas()->update();

                // Update parameters when canvas has changed
                connect(this->parameterWidget(),SIGNAL(dataModelChanged()),
                        this,SIGNAL(dataModelChanged()));
            }
        }
    }
}

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

#include "Mapping.h"
#include "ui_omni_ui_Mapping.h"

#include <QVBoxLayout>
#include <omni/Session.h>
#include <omni/mapping/Interface.h>

namespace omni {
    namespace ui {
        Mapping::Mapping(QWidget *_parent) :
            DockWidget(_parent)
        {
            this->setup(ui_);

            connect(ui_->boxMappingSelect, SIGNAL(currentIndexChanged(
                                                      QString)), this,
                    SLOT(selectMappingType(QString)));

            for (auto& _idMappingClass : omni::mapping::Factory::classes()) {
                QString _id = _idMappingClass.first.str();
                ui_->boxMappingSelect->addItem(QIcon(QString(":/mapping/") + _id +
                                                     QString(".png")), _id);
            }
        }

        Mapping::~Mapping()
        {}

        void Mapping::dataToFrontend()
        {
            if (!dataModel()->mapping())
            {
                dataModel()->setMapping("TexCoords");
            }

            // Search combobox for available mapping types
            int _index = 0;

            for (int i = 0; i < ui_->boxMappingSelect->count(); ++i)
            {
                QString _id = ui_->boxMappingSelect->itemData(i).toString();

                if (_id == dataModel()->mapping()->getTypeId().str())
                {
                    _index = i;
                }
            }

            ui_->boxMappingSelect->setCurrentIndex(_index);
            showParameterWidget();
            emit dataModelChanged();
        }

        /// Assign widget values to current mapping
        bool Mapping::frontendToData() {
            return false;
        }

        void Mapping::selectMappingType(QString const& _id)
        {
            if (!dataModel() || this->isLocked()) return;

            mappingMemory_.store(dataModel()->mapping());
            dataModel()->setMapping(_id);
            mappingMemory_.restore(dataModel()->mapping());

            showParameterWidget();
            emit dataModelChanged();
        }

        void Mapping::showParameterWidget() {
            this->setupParameterWidget(widget(), dataModel()->mapping());

            if (this->parameterWidget()) {
                connect(this->parameterWidget(), SIGNAL(
                            parametersUpdated()), this,
                        SIGNAL(dataModelChanged()));
            }
        }
    }
}

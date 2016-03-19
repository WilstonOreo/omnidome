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

namespace omni
{
  namespace ui
  {
    Mapping::Mapping(QWidget* _parent) :
        DockWidget(_parent)
    {
        this->setup(ui_);

      connect(ui_->boxMappingSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(selectMappingType(QString)));
       for (auto& _idMappingClass : omni::mapping::Factory::classes()) {
            QString _id = _idMappingClass.first.str();
            ui_->boxMappingSelect->addItem(QIcon(QString(":/mapping/")+ _id + QString(".png")),_id);
       }
      setDefaultMappingForCanvas();
    }

    Mapping::~Mapping()
    {
    }

    void Mapping::dataToFrontend()
    {
      if (!dataModel()->mapping())
      {
        dataModel()->setMapping("Equirectangular");
        setDefaultMappingForCanvas();
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
    }

    /// Assign widget values to current mapping
    bool Mapping::frontendToData() {
        return false;
    }

    void Mapping::selectMappingType(QString const& _id)
    {
      if (!dataModel() || this->isLocked()) return;

      if (paramWidget_) {
          widget()->layout()->removeWidget(paramWidget_);
      }

      dataModel()->setMapping(_id);

      auto* _mapping = dataModel()->mapping();
      if (!_mapping) return;

      paramWidget_ = _mapping->widget();

      if (paramWidget_) {
          // Configure layout
          widget()->layout()->addWidget(paramWidget_);

          // Update parameter when canvas has changed
          connect(paramWidget_,SIGNAL(parametersUpdated()),this,SIGNAL(dataModelChanged()));
          paramWidget_->show();
      }
      emit dataModelChanged();

      /// Remove widget on next mapping type change
      if (paramWidget_) {
          connect(this,SIGNAL(dataModelChanged()),paramWidget_,SLOT(deleteLater()));
      }
    }

     void Mapping::setDefaultMappingForCanvas() {
         if (!dataModel() || this->isLocked()) return;

         auto* _canvas = dataModel()->canvas();
         if (!_canvas) return;

         std::map<Id,int> _idToIndex;
         int _index = 0;

         for (auto& _idMappingClass : omni::mapping::Factory::classes())
         {
             QString _id = _idMappingClass.first.str();
             // Add mapping mode to combobox if it supported by canvas
            _idToIndex[_id] = _index;
            ++_index;
         }

         if (paramWidget_) {
             widget()->layout()->removeWidget(paramWidget_);
             delete paramWidget_;
         }

         for (auto& _idMappingClass : omni::mapping::Factory::classes()) {
             QString _id = _idMappingClass.first.str();
             auto&& _mapping = omni::mapping::Factory::create(_id);

             // Get type id of mapping and select it
             if (_mapping->getTypeId() == _canvas->defaultMappingMode()) {
                 ui_->boxMappingSelect->setCurrentIndex(_idToIndex[_id]);
                 selectMappingType(_mapping->getTypeId());
                 return;
             }
         }
     }
  }
}

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

      connect(ui_->boxMappingSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(mappingTypeSelected(QString)));
       for (auto& _idMappingClass : omni::mapping::Factory::classes()) {
            QString _id = _idMappingClass.first.str();
            ui_->boxMappingSelect->addItem(QIcon(QString(":/mapping/")+ _id + QString(".png")),_id);
       }
      setDefaultMappingForCanvas();
    }

    Mapping::~Mapping()
    {
    }

    void Mapping::sessionParameters()
    {
      if (!session()->mapping())
      {
        session()->setMapping("Equirectangular");
        setDefaultMappingForCanvas();
      }

      // Search combobox for available mapping types
      int _index = 0;
      for (int i = 0; i < ui_->boxMappingSelect->count(); ++i)
      {
        QString _id = ui_->boxMappingSelect->itemData(i).toString();
        if (_id == session()->mapping()->getTypeId().str())
        {
          _index = i;
        }
      }

      ui_->boxMappingSelect->setCurrentIndex(_index);
      emit mappingChanged();
    }

    void Mapping::mappingTypeSelected(QString const& _id)
    {
      if (!session() || this->isLocked()) return;

      session()->setMapping(_id);

      if (paramWidget_) {
          widget()->layout()->removeWidget(paramWidget_);
      }

      auto* _mapping = session()->mapping();
      if (!_mapping) return;

      paramWidget_ = _mapping->widget();//qobject_cast<MappingParameters*>(_mapping->widget());
      if (paramWidget_) {
          // Configure layout
          widget()->layout()->addWidget(paramWidget_);

          // Update parameter when canvas has changed
          connect(paramWidget_,SIGNAL(parametersUpdated()),this,SIGNAL(mappingChanged()));
          paramWidget_->show();
      }
      emit mappingTypeChanged();

      /// Remove widget on next mapping type change
      if (paramWidget_) {
          connect(this,SIGNAL(mappingTypeChanged()),paramWidget_,SLOT(deleteLater()));
      }
    }

     void Mapping::setDefaultMappingForCanvas() {
         if (!session() || this->isLocked()) return;

         auto* _canvas = session()->canvas();
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
                 mappingTypeSelected(_mapping->getTypeId());
                 return;
             }
         }
     }
  }
}

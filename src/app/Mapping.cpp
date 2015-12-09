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

#include <omni/ui/Mapping.h>

#include "ui_omni_ui_Mapping.h"

#include <QVBoxLayout>
#include <omni/Session.h>
#include <omni/mapping/Interface.h>

namespace omni
{
  namespace ui
  {
    Mapping::Mapping(QWidget* _parent) :
        CollapsibleGroupBox(_parent),
      ui_(new Ui::Mapping)
    {
        setTitle("Mapping");
      ui_->setupUi(widget());

      // Configure layout
      QLayout* _layout = new QVBoxLayout;
      _layout->setSpacing(2);
      _layout->setContentsMargins(0,0,0,0);
      ui_->widget->setLayout(_layout);

      // Update parameter when canvas has changed
      connect(ui_->widget,SIGNAL(parametersUpdated()),this,SIGNAL(mappingChanged()));

      connect(ui_->boxMappingSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(mappingTypeSelected(QString)));

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
      ui_->widget->setMapping(session()->mapping());
      emit mappingChanged();
    }

    void Mapping::mappingTypeSelected(QString const& _id)
    {
      if (!session() || this->isLocked()) return;

      session()->setMapping(_id);
      if (session()->mapping())
        ui_->widget->setMapping(session()->mapping());

      emit mappingTypeChanged();
    }

     void Mapping::setDefaultMappingForCanvas() {
         if (!session() || this->isLocked()) return;

         auto* _canvas = session()->canvas();
         if (!_canvas) return;

         ui_->boxMappingSelect->clear();

         auto _canvasMappingModes = _canvas->supportedMappingModes();
         std::map<Id,int> _idToIndex;
         int _index = 0;

         for (auto& _idMappingClass : omni::mapping::Factory::classes())
         {
             QString _id = _idMappingClass.first.str();
             auto&& _mapping = omni::mapping::Factory::create(_id);

             // Add mapping mode to combobox if it supported by canvas
             if (_canvasMappingModes.count(_mapping->mode())) {
                 _idToIndex[_id] = _index;
                 ui_->boxMappingSelect->addItem(QIcon(QString(":/mapping/")+ _id + QString(".png")),_id);
                 ++_index;
             }
         }

         for (auto& _idMappingClass : omni::mapping::Factory::classes()) {
             QString _id = _idMappingClass.first.str();
             auto&& _mapping = omni::mapping::Factory::create(_id);

             // Get type id of mapping and select it
             if (_mapping->mode() == _canvas->defaultMappingMode()) {
                 ui_->boxMappingSelect->setCurrentIndex(_idToIndex[_id]);
                 mappingTypeSelected(_mapping->getTypeId());
             }
         }


        emit mappingTypeChanged();
     }
  }
}

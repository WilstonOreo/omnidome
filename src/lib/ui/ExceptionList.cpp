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

#include <omni/ui/ExceptionList.h>

#include "ui_omni_ui_ExceptionList.h"

namespace omni {
  namespace ui {
    ExceptionList::ExceptionList(QWidget *_parent) :
      QDialog(_parent),
      ui_(new Ui::ExceptionList)
    {
      ui_->setupUi(this);
      setupModel();
    }

    ExceptionList::~ExceptionList() {}

    void ExceptionList::addException(Exception const& _exception) {
      QList<QStandardItem *> _row;
      auto _makeItem = [](QString const& _t) -> QStandardItem * {
                         QStandardItem *_item = new QStandardItem(_t);

                         _item->setEditable(false);
                         return _item;
                       };

      _row << _makeItem(_exception.typeAsString());
      _row << _makeItem(_exception.message());
      model_->invisibleRootItem()->appendRow(_row);
    }

    void ExceptionList::clear() {
      model_->clear();
    }

    /// Number of exceptions in list
    int ExceptionList::exceptionCount() const {
      return model_->rowCount();
    }

    void ExceptionList::setupModel() {
      model_.reset(new QStandardItemModel());
      model_->clear();
      model_->setColumnCount(2);

      model_->setHeaderData(0, Qt::Horizontal, "Type");
      model_->setHeaderData(1, Qt::Horizontal, "Message");

      ui_->exceptionList->setModel(model_.get());
    }
  }
}

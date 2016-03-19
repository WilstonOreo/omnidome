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

#include "Input.h"

#include "ui_omni_ui_Input.h"

#include <QMenu>
#include <QDebug>
#include <QVBoxLayout>

#include <omni/Session.h>
#include <omni/input/Interface.h>

namespace omni
{
  namespace ui
  {
    Input::Input(QWidget* _parent) :
        DockWidget(_parent)
    {
        this->setup(ui_);

      // Make popup menu for adding inputs
      {
        QMenu* _menu = new QMenu();
        ui_->btnAddInput->setMenu(_menu);
        for (auto& _idInputClass : omni::input::Factory::classes())
        {
          QString _id = _idInputClass.first.str();
          QAction* _menuItem = new QAction(QIcon(QString(":/input/")+ _id + QString(".png")),_id,_menu);
          _menu->addAction(_menuItem);
        }
        connect(_menu,SIGNAL(triggered(QAction*)),this,SLOT(addInput(QAction*)));
      }

      connect(ui_->inputList,SIGNAL(clicked(QModelIndex)),this,SLOT(changeSelection(QModelIndex)));
      connect(ui_->inputList,SIGNAL(activated(QModelIndex)),this,SLOT(changeSelection(QModelIndex)));
      connect(ui_->btnRemoveInput,SIGNAL(clicked()),this,SLOT(removeSelection()));

      prepareModel();
    }

    Input::~Input()
    {
    }

    void Input::dataToFrontend()
    {
      prepareModel();
      if (!dataModel()) {
            if (paramWidget_) {
                paramWidget_->deleteLater();
                paramWidget_ = nullptr;
            }
          return;
      }

      for (auto& _input : dataModel()->inputs())
      {
        addItem(_input.first,_input.second.get());
      }
      setupInputWidget();
    }

    void Input::addInput(QAction* _action)
    {
      if (!dataModel()) return;

      auto _idInput = dataModel()->inputs().add(_action->text());
      auto& _id = _idInput.first;
      auto& _input = _idInput.second;

      if (!_input) return;

      if (_input->canAdd())
      {
        addItem(_id,_input);
        dataModel()->inputs().setCurrentId(_id);
        paramWidget_ = _input->widget();
        setupInputWidget();
      } else
      {
        dataModel()->inputs().remove(_id);
      }
    }

    void Input::removeSelection()
    {
      if (dataModel()->inputs().empty()) return;

      int _row = ui_->inputList->currentIndex().row();

      qDebug() << "removeSelection: " << _row;
      if (_row < 1 || _row > dataModel()->inputs().size()) return;

      auto* _item = model_->item(_row);
      qDebug() << "removeSelection: " << _item;
      if (!_item) return;

      dataModel()->inputs().remove(_item->text());
      model_->removeRows(_row,1);
      if (paramWidget_) {
          paramWidget_->hide();
      }
      changeSelection(model_->index(_row,0));

      emit inputChanged();
    }

    void Input::clear()
    {
      dataModel()->inputs().clear();
      prepareModel();
      setupInputWidget();
    }

    QString Input::itemId(int _row) const {
      auto* _item = model_->item(_row);
      if (!_item) return QString();
      return _item->text();
    }

    void Input::changeSelection(QModelIndex _index)
    {
      if (!dataModel()) return;
      int _row = _index.row();

      if (_row < 1 || _row > dataModel()->inputs().size())
      {
          _row = -1;
      }
      qDebug() << "changeSelection: " << itemId(_row);

      dataModel()->inputs().setCurrentId(itemId(_row));
      setupInputWidget();
    }

    void Input::setupInputWidget() {
        auto* _input = dataModel()->inputs().current();
        if (!_input) {
            if (paramWidget_) {
                paramWidget_->deleteLater();
                paramWidget_ = nullptr;
            }
            emit inputIndexChanged();
            return;
        }

        paramWidget_ = dataModel()->inputs().current()->widget();
        if (paramWidget_) {
            QSizePolicy _sizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            _sizePolicy.setVerticalStretch(1);
            paramWidget_->setSizePolicy(_sizePolicy);
            paramWidget_->setMinimumSize(QSize(0,128));

            // Configure layout
            widget()->layout()->addWidget(paramWidget_);

            emit inputChanged();

            // Update parameter when canvas has changed
            connect(paramWidget_,SIGNAL(inputChanged()),this,SIGNAL(inputChanged()));
        }

        emit inputIndexChanged();

        ///   Remove widget on next mapping type change
        if (paramWidget_) {
            connect(this,SIGNAL(inputIndexChanged()),paramWidget_,SLOT(deleteLater()));
        }
    }

    void Input::prepareModel()
    {
      model_.reset(new QStandardItemModel());
      model_->clear();
      model_->setColumnCount(3);
      model_->setHeaderData(0,Qt::Horizontal,"Id");
      model_->setHeaderData(1,Qt::Horizontal,"Input Type");
      model_->setHeaderData(2,Qt::Horizontal,"Info");
      ui_->inputList->setModel(model_.get());

      QList<QStandardItem*> _row;
      QStandardItem* _hash = new QStandardItem("#");
      _hash->setEditable(false);
      QStandardItem* _item = new QStandardItem("(no input)");
      _item->setEditable(false);
      _row << _hash;
      _row << _item;
      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);
      ui_->inputList->resizeColumnToContents(2);

      if (paramWidget_) {
         paramWidget_->deleteLater();
         paramWidget_ = nullptr;
      }
    }

    void Input::addItem(QString const& _id, input::Interface const* _input)
    {
      if (!_input) return;

      QList<QStandardItem*> _row;
      auto _addItem = [&](QString const& _id) {
          QStandardItem* _item = new QStandardItem(_id);
          _item->setEditable(false);
          _row << _item;
      };

      _addItem(_id);
      _addItem(_input->getTypeId().str());
      _addItem(_input->infoText());

      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);
      ui_->inputList->resizeColumnToContents(2);
    }
  }
}

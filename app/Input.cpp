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
        setupInputMenu(_menu);
        ui_->btnAddInput->setMenu(_menu);
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


    void Input::setupInputMenu(QMenu* _menu)
    {
      std::map<QString,QMenu*> _categoryMenus;
      /// Find all categories
      for (auto& _idInputClass : omni::input::Interface::factory().classes())
      {
        auto* _input = omni::input::Interface::factory().create(_idInputClass.first);
        if (!_input) continue;

        auto& _id = _idInputClass.first;

        auto _categories = _input->categories();
        if (_categories.empty())
        {
          QAction* _menuItem = new QAction(QIcon(QString(":/input/")+ _id + QString(".png")),_id,_menu);
          _menu->addAction(_menuItem);
        }

        for (auto& _category : _categories)
        {
          if (!_categoryMenus.count(_category))
          {
            _categoryMenus[_category] = new QMenu(_category);
          }
          auto* _catMenu = _categoryMenus[_category];
          QAction* _menuItem = new QAction(QIcon(QString(":/input/")+ _id + QString(".png")),_id,_menu);
          _catMenu->addAction(_menuItem);
        }
      }

      /// No categories exist
      for (auto& _categoryMenu : _categoryMenus)
      {
        _menu->addMenu(_categoryMenu.second);
      }
    }

    void Input::dataToFrontend()
    {
      prepareModel();
      for (auto& _input : dataModel()->inputs())
      {
        addItem(_input.get());
      }
      selectInput(dataModel()->inputs().currentIndex());
      showParameterWidget();
      emit inputIndexChanged();
    }

    void Input::addInput(QAction* _action)
    {
      if (!dataModel()) return;

      auto _idInput = dataModel()->inputs().addInput(_action->text().toLatin1());
      auto& _input = _idInput;

      if (!_input) return;

      if (_input->canAdd())
      {
        addItem(_input);
        selectInput(dataModel()->inputs().size()-1);
        showParameterWidget();
        emit inputIndexChanged();
      }
      else
      {
        dataModel()->inputs().removeInput(dataModel()->inputs().size()-1);
      }
    }

    void Input::selectInput(int index)
    {
      dataModel()->inputs().setCurrentIndex(index);
      auto* _current = dataModel()->inputs().current();
      if (_current)
      {
        _current->update();
        emit inputChanged();
      }
    }

    void Input::inputUpdatedEmitter()
    {
      emit inputChanged();
    }

    void Input::removeSelection()
    {
      if (dataModel()->inputs().empty()) return;

      int _row = ui_->inputList->currentIndex().row();

      if (_row < 1 || _row > dataModel()->inputs().size()) return;

      auto* _item = model_->item(_row);
      if (!_item) return;

      dataModel()->inputs().removeInput(_row - 1);
      model_->removeRows(_row,1);

      this->removeParameterWidget(widget());
      changeSelection(model_->index(_row,0));
      emit inputRemoved();
    }

    void Input::clear()
    {
      if (!dataModel()) return;
      dataModel()->inputs().clear();
      prepareModel();
      showParameterWidget();
      emit inputIndexChanged();
    }

    void Input::changeSelection(QModelIndex _index)
    {
      if (!dataModel() || signalsBlocked()) return;
      int _row = _index.row();

      if (_row < 1 || _row > dataModel()->inputs().size())
      {
        _row = -1;
      }

      if (_row-1 == dataModel()->inputs().currentIndex()) return;

      selectInput(_row - 1);
      showParameterWidget();
      emit inputIndexChanged();
    }
    void Input::showParameterWidget()
    {

      this->setupParameterWidget(widget(),dataModel()->inputs().current());
      if (this->parameterWidget())
      {
        connect(this->parameterWidget(),SIGNAL(inputChanged()),this,SIGNAL(inputChanged()));
        connect(this,SIGNAL(inputChanged()),this->parameterWidget(),SLOT(triggerUpdate()));
      }
    }

    void Input::prepareModel()
    {
      this->removeParameterWidget(widget());

      model_.reset(new QStandardItemModel());
      model_->clear();
      model_->setColumnCount(2);
      model_->setHeaderData(0,Qt::Horizontal,"Input Type");
      model_->setHeaderData(1,Qt::Horizontal,"Info");
      ui_->inputList->setModel(model_.get());

      QList<QStandardItem*> _row;
      QStandardItem* _item = new QStandardItem("(no input)");
      _item->setEditable(false);
      _row << _item;
      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);
    }

    void Input::addItem(input::Interface* _input)
    {
      if (!_input) return;

      QList<QStandardItem*> _row;
      auto _addItem = [&](QString const& _id)
      {
        QStandardItem* _item = new QStandardItem(_id);
        _item->setEditable(false);
        _row << _item;
      };

      _addItem(_input->getTypeId());
      _addItem(_input->infoText());

      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);

      _input->setUpdateCallback(std::bind(&Input::inputUpdatedEmitter,this));
    }

    void Input::parameterWidgetSetupOptions(QWidget* _paramWidget) const
    {
      QSizePolicy _sizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
      _sizePolicy.setVerticalStretch(1);
      _paramWidget->setSizePolicy(_sizePolicy);
      _paramWidget->setMinimumSize(QSize(0,128));
    }
  }
}

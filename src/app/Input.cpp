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

    Session const* Input::session() const
    {
      return session_;
    }

    void Input::setSession(Session* _session)
    {
      //ui_->preview->hide();
      session_=_session;
      //ui_->preview->setSession(_session);

      prepareModel();
      for (auto& _input : session_->inputs())
      {
        addItem(_input.get());
      }
      //ui_->preview->setVisible(session_->inputs().current() != nullptr);

      emit inputChanged();
    }

    void Input::addInput(QAction* _action)
    {
      if (!session_) return;

      auto* _input = session_->inputs().add(_action->text());
      if (!_input) return;

      if (_input->canAdd())
      {
        addItem(_input);
        session_->inputs().setCurrentIndex(session_->inputs().size()-1);

        paramWidget_ = _input->widget();
        setupInputWidget();
      } else
      {
        session_->inputs().remove(session_->inputs().size()-1);
      }
    }

    void Input::removeSelection()
    {
      if (session_->inputs().empty()) return;

      int _row = ui_->inputList->currentIndex().row() - 1;
      if (_row < 0 || _row >= session_->inputs().size()) return;

      session_->inputs().remove(_row);
      model_->removeRows(_row+1,1);
      changeSelection(model_->index(_row-1,0));

      emit inputChanged();
    }

    void Input::clear()
    {
      session_->inputs().clear();
      prepareModel();
      setupInputWidget();
      emit inputChanged();
    }

    void Input::changeSelection(QModelIndex _index)
    {
      if (!session_) return;
      if (_index.row() == session_->inputs().currentIndex()+1) return;

      int _row = _index.row() - 1;
      qDebug() << _row;

      if (_row < 0 || _row >= session_->inputs().size())
      {
          _row = -1;
      }

      session_->inputs().setCurrentIndex(_row);
      setupInputWidget();
    }

    void Input::setupInputWidget() {
        auto* _input = session_->inputs().current();
        if (!_input) {
            if (paramWidget_) {
                paramWidget_->deleteLater();
                paramWidget_ = nullptr;
            }
            emit inputIndexChanged();
            return;
        }

        paramWidget_ = session_->inputs().current()->widget();
        if (paramWidget_) {
            QSizePolicy _sizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            _sizePolicy.setVerticalStretch(1);
            paramWidget_->setSizePolicy(_sizePolicy);
            paramWidget_->setMinimumSize(QSize(0,128));

            // Configure layout
            widget()->layout()->addWidget(paramWidget_);

            paramWidget_->show();
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

    void Input::addItem(input::Interface const* _input)
    {
      if (!_input) return;

      QList<QStandardItem*> _row;
      QStandardItem* _item = new QStandardItem(_input->getTypeId());
      _item->setEditable(false);
      _row << _item;
      _item = new QStandardItem(_input->infoText());
      _item->setEditable(false);
      _row << _item;

      model_->invisibleRootItem()->appendRow(_row);
      ui_->inputList->resizeColumnToContents(0);
      ui_->inputList->resizeColumnToContents(1);
    }
  }
}

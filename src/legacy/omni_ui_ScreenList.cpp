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

#include <omni/ui/ScreenList.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>
#include <iostream>

using namespace omni::ui;

ScreenList::ScreenList(QWidget* _parent) :
  QTreeView(_parent),
  model_(nullptr)
{
}

Fullscreen* ScreenList::fullscreen(size_t _id)
{
  if (_id >= fullscreenWidgets_.size()) return nullptr;
  return fullscreenWidgets_[_id].get();
}

omni::Screen const* ScreenList::screen(size_t _id) const
{
  if (_id >= screens_.size()) return nullptr;
  return &screens_[_id];
}

size_t ScreenList::count()
{
  return screens().size();
}


void ScreenList::prepareModel()
{
  model_.reset(new QStandardItemModel());
  setModel(model_.get());
  model_->clear();
  model_->setColumnCount(3);
  model_->setHeaderData(0,Qt::Horizontal,"Id");
  model_->setHeaderData(1,Qt::Horizontal,"Resolution");
  model_->setHeaderData(2,Qt::Horizontal,"Subscreens");

}

ScreenList::~ScreenList()
{
}


void ScreenList::closeEvent(QCloseEvent* _event)
{
  clear();
}

void ScreenList::clear()
{
  screens_.clear();
  fullscreenWidgets_.clear();
}

void ScreenList::identify(bool _identify)
{
  for (auto& _fullscreenWidget : fullscreenWidgets_)
  {
    _fullscreenWidget->identify(_identify);
  }
}

void ScreenList::updateScreens()
{
  clear();
  prepareModel();
  this->expandAll();
  this->header()->setStretchLastSection(false);
//  this->header()->setResizeMode(1, QHeaderView::Stretch); // deprectated in Qt5
  QDesktopWidget* _desktop = QApplication::desktop();
  int _numberScreens = _desktop->screenCount();

  int _index = 0;
  for (int i = 0; i < _numberScreens; ++i)
  {
    QRect _screenRect = _desktop->screenGeometry(i);
    if (_screenRect == _desktop->screenGeometry(this)) continue;

    screens_.emplace_back(
      _screenRect.width(),
      _screenRect.height(),
      _screenRect.x(),
      _screenRect.y(),
      _index);

    size_t _subScreenCount = screens_.back().subScreenCount();
    addItem(model_->invisibleRootItem(),
            std::vector<QStandardItem*>(
    {
      new QStandardItem(tr("%1").arg(i)),
      new QStandardItem(tr("%1 x %2").arg(_screenRect.width()).arg(_screenRect.height())),
      new QStandardItem(tr("%1").arg(_subScreenCount))
    }));
    fullscreenWidgets_.emplace_back(new Fullscreen(screens_.back()));

    _index++;
  }
}


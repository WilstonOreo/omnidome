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

#include "ScreenSetup.h"
#include "proj/TuningList.h"

#include <omni/Session.h>
#include "TuningGLView.h"
#include <omni/proj/ScreenSetup.h>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QGuiApplication>

#include "proj/Tuning.h"

namespace omni
{
  namespace ui
  {
    ScreenSetup::ScreenSetup(QWidget* _parent) :
      QWidget(_parent),
      TransformedRect<ScreenSetup>(this)
    {
      setMouseTracking(true);
      setAcceptDrops(true);
      setAutoFillBackground(true);

      connect(QGuiApplication::instance(),SIGNAL(screenAdded(QScreen*)),
          this,SLOT(updateScreens()));
      connect(QGuiApplication::instance(),SIGNAL(screenRemoved(QScreen*)),
          this,SLOT(updateScreens()));

      updateScreens();
    }

    ScreenSetup::~ScreenSetup()
    {
      screenItems_.clear();
    }

    void ScreenSetup::setTuningList(proj::TuningList* _list) {
      tuningList_ = _list;
     // connect(tuningList_,SIGNAL(tuningAdded()),this,SLOT(assignNewTuningToNextFreeScreen()));
    }

    proj::TuningList* ScreenSetup::tuningList() {
      return tuningList_;
    }

    proj::TuningList const* ScreenSetup::tuningList() const {
      return tuningList_;
    }

    void ScreenSetup::dataToFrontend() {
      updateScreens();
    }

    void ScreenSetup::updateScreens()
    {
      screenItems_.clear();

      auto _screens = omni::proj::ScreenSetup::screens();

      for (auto& _screen : _screens)
      {
        screenItems_[_screen].reset(new ScreenItem(*this,_screen));
      }

      if (tuningList_) {
        for (auto& _tuning : dataModel()->tunings()) {
          assignTuning(_tuning.get());
        }
      }
      update();
    }
    
    void ScreenSetup::updateViews() {
      for (auto& _screenItem : screenItems_) {
        _screenItem.second->fullscreen()->update();
      }
    }

    void ScreenSetup::closeFullscreenWindows() {
      for (auto& _screenItem : screenItems_) {
        _screenItem.second->fullscreen()->close();
        _screenItem.second->fullscreen()->deleteLater();
      }
      screenItems_.clear();
    }

    void ScreenSetup::assignNewTuningToNextFreeScreen() {
      auto* _tuning = dataModel()->tunings()[dataModel()->tunings().size()-1];
      if (!_tuning) return;

      /// Iterate through all screens and subscreens
      for (auto& _screenItem : screenItems_) {
          auto* _item = _screenItem.second.get();
          auto* _screen = _screenItem.first;
          int _numSubScreens = _item->numSubScreens();
          for (int _subScreenIndex = 0; _subScreenIndex < _numSubScreens; ++_subScreenIndex) {
            auto* _subScreenItem = _item->item(_subScreenIndex);
            if (_subScreenItem->tuningWidget()) continue;

            // If sub screen item has no tuning widget assigned its free.
            // So we can assign the tuning here
            _tuning->setScreen(_screen,_subScreenIndex);
            assignTuning(_tuning);
            this->update();
            return; // Nothing more to do
          }
      }
    }

    void ScreenSetup::assignTuning(omni::proj::Tuning* _tuning) {
      if (!_tuning->screen()) return; // No assignment for default screen
      if (!screenItems_.count(_tuning->screen())) {
        _tuning->assignVirtualScreen();
        return;
      }

      auto* _subScreenItem = screenItems_.at(_tuning->screen())->item(_tuning->subScreenIndex());
      if (!_subScreenItem) return;

      auto* _tuningWidget = tuningList_->widgetFromTuning(_tuning);
      if (!_tuningWidget) return;
      _subScreenItem->attachTuningWidget(_tuningWidget);
    }

    void ScreenSetup::paintEvent(QPaintEvent*)
    {
      QPainter _p(this);
      _p.setPen(QPen(QColor("#ffffff").darker(150),3));

      if (screenItems_.empty())
      {
        QFont _font("Helvetica",30 / devicePixelRatio());
        _p.setFont(_font);
        _p.setBrush(QBrush("#cccccc"));
        _p.drawText(rect(),Qt::AlignCenter,"No screen connected.");
        return;
      }

      _p.setPen(QPen(QColor("#ffffff").darker(150),3));
      _p.drawRect(transformedRect());

      /// Draw all items
      for (auto& _item : screenItems_)
      {
        _item.second->paint(_p);
      }
    }

    void ScreenSetup::mouseMoveEvent(QMouseEvent* _event)
    {
      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        _item->setHoverIndex(_event->pos());
      }
      update();
    }

    void ScreenSetup::mouseDoubleClickEvent(QMouseEvent* _event)
    {
      auto _subScreenItem = getSubScreenItemAtPos(_event->pos());
      if (_subScreenItem)
      {
        _subScreenItem->detachTuningWidget();
        update();
      }
    }

    SubScreenItem* ScreenSetup::getSubScreenItemAtPos(QPoint const& _pos)
    {
      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        _item->setHoverIndex(_pos);
        if (_item->hoveredItem())
        {
          return _item->hoveredItem();
        }
      }
      return nullptr;
    }

    void ScreenSetup::dragEnterEvent(QDragEnterEvent* event)
    {
      currentSubScreenItem_ = getSubScreenItemAtPos(event->pos());
      for (auto& _item : screenItems_)
        _item.second->endDrop();

      if (event->mimeData()->hasFormat("text/plain"))
      {
          auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
          if (!_tuningWidget->tuning()) return;

          if (currentSubScreenItem_) {
            auto _color = _tuningWidget->tuning()->color();
            currentSubScreenItem_->parent()->startDrop(_color);
          }
          event->acceptProposedAction();
      }

      if (currentSubScreenItem_) {
        update();
      }
    }

    void ScreenSetup::dragMoveEvent(QDragMoveEvent* event)
    {
      auto* _screenItem = getSubScreenItemAtPos(event->pos());
      if (_screenItem == currentSubScreenItem_) {
        return;
      }

      for (auto& _item : screenItems_)
        _item.second->endDrop();

      currentSubScreenItem_ = _screenItem;

      if (event->mimeData()->hasFormat("text/plain"))
      {
          auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
          if (!_tuningWidget->tuning()) return;

          if (currentSubScreenItem_) {
            auto _color = _tuningWidget->tuning()->color();
            currentSubScreenItem_->parent()->startDrop(_color);
          }
          event->acceptProposedAction();
      }
      if (currentSubScreenItem_) {
        update();
      }
    }

    void ScreenSetup::dropEvent(QDropEvent* event)
    {
      for (auto& _item : screenItems_)
        _item.second->endDrop();

      currentSubScreenItem_ = getSubScreenItemAtPos(event->pos());

      if (currentSubScreenItem_)
      {
        auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
        if (!_tuningWidget->tuning()) return;

        // Detach tuning from all screens if already assigned
        for (auto& _screenItem : screenItems_) {
            _screenItem.second->detachTuningWidget(_tuningWidget);
        }

        currentSubScreenItem_->attachTuningWidget(_tuningWidget);
        event->acceptProposedAction();
      }

      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        _item->endDrop();
      }

      currentSubScreenItem_ = nullptr;
    }
  }
}

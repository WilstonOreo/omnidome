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

#include "FullScreen.h"
#include "ScreenSetup.h"
#include <omni/proj/ScreenSetup.h>

namespace omni
{
  namespace ui
  {
    FullScreen::FullScreen(const QScreen* _screen) :
      screen_(_screen),
      dragWidget_(new ScreenSetupDragWidget(this))
    {
      setStyleSheet("* { background:black; }");
      setWindowFlags( Qt::CustomizeWindowHint );
      setWindowFlags(Qt::FramelessWindowHint);

      move(_screen->geometry().topLeft());
      this->hide();
    }

    FullScreen::~FullScreen()
    {
      detachAll();
    }

    void FullScreen::detachAll()
    {
      for (auto& _widget : widgets_)
      {
        _widget.second->hide();
        _widget.second->setParent(nullptr);
      }
      widgets_.clear();
      hide();
    }

    void FullScreen::detach(int _subIndex)
    {
      if (widgets_.count(_subIndex) == 0) return;

      widgets_[_subIndex]->hide();
      widgets_[_subIndex]->setParent(nullptr);
      widgets_.erase(_subIndex);
      if (widgets_.empty())
        hide();
    }

    void FullScreen::attach(int _subIndex, QWidget* _widget)
    {
      if (widgets_.count(_subIndex) > 0)
      {
        detach(_subIndex);
      }

      widgets_[_subIndex] = _widget;

      setGeometry(_widget,_subIndex);
      _widget->setParent(this);
      _widget->show();
      showFullScreen();
    }

    void FullScreen::hideDragWidget()
    {
      dragWidget_->hide();
      if (widgets_.empty())
        hide();
    }

    void FullScreen::setGeometry(QWidget* _widget, int _subIndex) const
    {
      int _subScreenCount = omni::proj::ScreenSetup::subScreenCount(screen_);

      if (_subIndex <  0) _subIndex = 0;
      if (_subIndex >= _subScreenCount) _subIndex = _subScreenCount - 1;

      _widget->setGeometry(omni::proj::ScreenSetup::subScreenRect(screen_,_subIndex));
    }

    void FullScreen::showDragWidget(int _subIndex, QColor const& _color)
    {
      setGeometry(dragWidget_.get(),_subIndex);
      dragWidget_->setColor(_color);
      dragWidget_->show();
      dragWidget_->raise();
      showFullScreen();
    }

    void FullScreen::closeEvent(QCloseEvent* _event)
    {
      detachAll();
    }
  }
}

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

#include <omni/ui/Fullscreen.h>

#include <omni/debug.hpp>

using namespace omni::ui;

Fullscreen::Fullscreen(const proj::Screen& _screen) :
  screen_(_screen)
{
  setWindowFlags( Qt::CustomizeWindowHint );
  setWindowFlags(Qt::FramelessWindowHint);
  move(_screen.x(),_screen.y());
  this->hide();
  transparent(false);

  size_t _subScreenCount = screen().subScreenCount();
  for (size_t _subIndex = 0; _subIndex < _subScreenCount; ++_subIndex)
  {
    size_t _w = screen_.width() / _subScreenCount;
    identifiers_.emplace_back(new ScreenIdentifier(screen().index(),_subIndex));
    identifiers_.back()->setGeometry(_w*_subIndex,0,_w,screen().height());
    identifiers_.back()->setParent(this);
  }
}

Fullscreen::~Fullscreen()
{
  /* FIXME: Crashes when screens are changing
   * Possible solution: Handle QScreenChangedEvent properly
   * for (auto& _idWidget : widgets_)
        {
          detach(_idWidget.first);
        }*/
}

void Fullscreen::transparent(bool _transparent)
{
  transparent_=_transparent;
  if (_transparent)
  {
    setAttribute(Qt::WA_TranslucentBackground,_transparent);
    setStyleSheet("* {background:transparent; }");
  }
  else
  {
    setStyleSheet("* { background:black; }");
  }
}

void Fullscreen::clear()
{
  for (size_t _subIndex = 0; _subIndex < screen().subScreenCount(); ++_subIndex)
  {
    detach(_subIndex);
  }
}

void Fullscreen::identify(bool _identify)
{
  OMNI_DBG("Fullscreen" << screen().index())
  showFullScreen();
  raise();
  for (auto& _identifier : identifiers_)
  {
    _identifier->setVisible(_identify);
  }
  for (auto& _idWidget : widgets_)
  {
    auto _widget = _idWidget.second;
    if (!_widget) continue;
    _widget->setVisible(!_identify);
  }
}

void Fullscreen::detach(size_t _subIndex)
{
  if (widgets_.count(_subIndex) == 0) return;

  widgets_[_subIndex]->hide();
  widgets_[_subIndex]->setParent(nullptr);
  widgets_.erase(_subIndex);
}

void Fullscreen::place(size_t _subIndex, QWidget* _widget)
{
  if (widgets_.count(_subIndex) > 0)
  {
    detach(_subIndex);
  }

  widgets_[_subIndex] = _widget;
  size_t _subScreenCount = screen_.subScreenCount();
  size_t _w = screen_.width() / _subScreenCount;
  _widget->setParent(this);
  _widget->setGeometry(_w*_subIndex,0,_w,screen_.height());
  _widget->show();
}

void Fullscreen::closeEvent(QCloseEvent* _event)
{
  for (auto& _idWidget : widgets_)
  {
    detach(_idWidget.first);
  }
}

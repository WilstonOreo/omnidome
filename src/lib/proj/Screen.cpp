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

#include <omni/proj/Screen.h>

namespace omni
{
  namespace proj
  {
    Screen::Screen(QRect const& _rect, int _index) :
      rect_(_rect),
      index_(_index)
    {}

    bool Screen::hasSubScreens() const
    {
      return subScreenCount() > 1;
    }

    int Screen::width() const
    {
      return rect_.width() / subScreenCount();
    }

    int Screen::height() const
    {
      return rect_.height();
    }
      
    qreal Screen::aspectRatio() const
    {
      return width() / qreal(height());
    }

    int Screen::subScreenCount() const
    {
      return subScreenCount(rect_);
    }
      
    int Screen::subScreenCount(QRect const& _rect)
    {
      return 3;/*
      auto _s = _rect.size();
      qreal _aspectRatio = _s.width() / qreal(_s.height());

      if (_aspectRatio < 1.5) return 1;

      // Detect triple head
      size_t _subscreenCount = 3;
      while (_rect.width() % _subscreenCount == 0)
      {
        --_subscreenCount;
        if (_subscreenCount == 1) break;
      }
      if (_subscreenCount <= 0) 
        _subscreenCount = 1;
      return _subscreenCount;*/
    }

    QRect Screen::rect() const
    {
      return rect_;
    }
  }
}

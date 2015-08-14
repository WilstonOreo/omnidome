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

#ifndef OMNI_PROJ_SCREEN_H_
#define OMNI_PROJ_SCREEN_H_

#include <QScreen>

namespace omni
{
  namespace proj
  {
    /// Screen class 
    struct Screen
    {
      /// Screen takes a QRect and an index as input
      Screen(QRect const&, int _index = 0);

      /// Returns true if number of subscreens is larger than 1
      bool hasSubScreens() const;

      /// Aspect ratio  
      qreal aspectRatio() const;
      
      /// Screen width
      int width() const;

      /// Screen height
      int height() const;

      /// Calculate number of subscreens, for triple head detection
      int subScreenCount() const;

      /// Calculate number of subscreens, for triple head detection from a given rectangle
      static int subScreenCount(QRect const&);

      /// Screen rectangle (absolute positioning)
      QRect rect() const;

    private:
      QRect rect_;
      int index_;
    };
  }

  using proj::Screen;
}

#endif /* OMNI_PROJ_SCREEN_H_ */

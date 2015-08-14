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

#ifndef OMNI_PROJ_SCREENSETUP_H_
#define OMNI_PROJ_SCREENSETUP_H_

#include <vector>
#include <QScreen>

namespace omni
{
  namespace proj
  {
    /// Singleton class which manages the current screen setup
    class ScreenSetup
    {
    public:
      ScreenSetup();

      /// Returns screen size that supports triple heads
      static std::vector<QSize> const& screenResolutions();

      /// Return standard screen, where omnidome has to be placed
      static QScreen const* standardScreen();

      /// Returns the number of subscreens for a single screen
      static int subScreenCount(QScreen const*);
      
      /// Returns the rectangle of a subscreen with a certain index
      static QRect subScreenRect(QScreen const*, int _subScreenIndex);
      static int subScreenWidth(QScreen const*);

      static QRect desktopRect(bool _excludeStandardScreen = true);
      static std::vector<QScreen const*> screens(bool _excludeStandardScreen = true);

      bool operator==(const ScreenSetup&) const;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */

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
  class Session;

  namespace proj
  {
    class Tuning;

    /// Singleton class which manages the current screen setup
    class ScreenSetup
    {
    public:
      ScreenSetup(Session const* _session);

      /// Returns screen size that supports triple heads
      static std::vector<QSize> const& screenResolutions();

      /// Return standard screen, where omnidome has to be placed
      static QScreen const* standardScreen();

      /// Return const pointer to session
      Session const* session() const;

      /// Returns the number of subscreens for a single screen
      static int subScreenCount(QScreen const*);

      /// Returns the rectangle of a subscreen with a certain index
      static QRect subScreenRect(QScreen const*, int _subScreenIndex);
      static int subScreenWidth(QScreen const*);

      static QRect desktopRect(bool _excludeStandardScreen = true);

      /// Virtual desktop rect contains all non-assigned tunings
      QRect virtualDesktopRect() const;

      /// Returns combined desktop and virtual desktop rect
      QRect combinedDesktopRect() const;

      /// Returns true if no tuning is assigned to a screen
      bool noTuningsAssigned() const;

      /// Return vector, optionally excluded standard screen
      static std::vector<QScreen const*> screens(bool _excludeStandardScreen = true);

      /**@brief Return screen rectangle for tuning, in combined desktop rect
         @param _tuning Tuning
         œparam _shrink Removes empty space from rect
       **/
      QRect tuningRect(proj::Tuning const* _tuning) const;

      /// Bounding rect that unifies all tunings
      QRect tuningRect() const;

      bool operator==(const ScreenSetup&) const;
    private:
      Session const* session_;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */

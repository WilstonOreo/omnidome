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

#include <set>
#include <vector>
#include <QScreen>
#include <omni/proj/ScreenMultiplex.h>

namespace omni {
  class Session;

  namespace proj {
    class Tuning;

    /// Singleton class which manages the current screen setup
    class ScreenSetup {
      public:
        ScreenSetup(Session const& _session);

        /**@brief Returns the screen geometry for a screen
           @detail If screen is null, the geometry for the virtual screen is returned
         **/
        QRect screenGeometry(QScreen const* = nullptr) const;

        /// Return standard screen, where omnidome has to be placed
        static QScreen const          * standardScreen();

        /// Return const pointer to session
        Session const                 & session() const;

        /// Return screen multiplex of given screen 
        ScreenMultiplex screenMultiplex(QScreen const*) const;

        /// Switches to the next possible tiling
        void setScreenMultiplex(QScreen const*, ScreenMultiplex const&);

        /// Return screen aspect ratio
        float subScreenAspectRatio(QScreen const*, int _index = 0) const;

        static QRect desktopRect(
          bool _excludeStandardScreen = true);

        /// Virtual desktop rect contains all non-assigned tunings
        QRect virtualScreenRect() const;

        /// Returns combined desktop and virtual desktop rect
        QRect combinedDesktopRect() const;

        /// Returns true if no tuning is assigned to a screen
        bool  noTuningsAssigned() const;

        bool  noTuningsAssigned(QScreen const *)
        const;

        std::set<QScreen const*> usedScreens() const;

        /// Return vector, optionally excluded standard screen
        static std::vector<QScreen const *>screens(
          bool _excludeStandardScreen = true);

        std::set<proj::Tuning const *>     tunings(bool _excludeNonAssigned) const;

        /// Return pointer to screen for rectangle, nullptr if no screen with
        // rectangle exists
        static QScreen const             * screenFromRect(QRect const&);

        /// Bounding rect that unifies all tunings
        QRect tuningRect() const;

        bool  operator==(const ScreenSetup&) const;

      private:
        Session const& session_;
        std::map<QScreen const*,ScreenMultiplex> multiplex_;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */

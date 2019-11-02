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

#ifndef OMNI_PROJ_SCREENSETUP_H_
#define OMNI_PROJ_SCREENSETUP_H_

#include <set>
#include <vector>
#include <QScreen>
#include <omni/global.h>

namespace omni {
  class Session;

  namespace proj {
    class Tuning;

    /// Singleton class which manages the current screen setup
    class OMNI_EXPORT ScreenSetup {
      public:
        ScreenSetup(Session const *_session);

        /**@brief Returns the screen geometry for a screen
           @detail If screen is null, the geometry for the virtual screen is returned
         **/
        QRect screenGeometry(QScreen const* = nullptr) const;

        /// Returns screen size that supports triple heads
        static std::vector<QSize>const& screenResolutions();

        /// Return standard screen, where omnidome has to be placed
        static QScreen const          * standardScreen();

        /// Return const pointer to session
        Session const                 * session() const;

        /**@brief Returns the number of subscreens for a single screen
           @detail If screen is nullptr, number of subscreen for virtual screen
                   is returned.
         **/
        int   subScreenCount(QScreen const * = nullptr) const;

        /** @brief Static method for getting subscreen count for screen
            @detail Screen must not be nullptr!
         **/
        static int   subScreenCountForScreen(QScreen const *);

        /**@brief Returns the number of subscreens for a single screen
           @detail If screen is nullptr, number of subscreen for virtual screen
                   is returned.
         **/
        int   subScreenWidth(QScreen const * = nullptr) const;

        /** @brief Static method for getting subscreen width for screen
            @detail Screen must not be nullptr!
         **/
        static int   subScreenWidthForScreen(QScreen const *);

        /// Returns the rectangle of a subscreen with a certain index
        QRect subScreenRect(int _index, QScreen const* = nullptr) const;

        /** @brief Static method for getting subscreen count for screen
            @detail Screen must not be nullptr!
         **/
        static QRect   subScreenRectForScreen(int _index, QScreen const *);

        /// Return aspect ratio of subscreen
        qreal subScreenAspectRatio(QScreen const* = nullptr) const;


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
        Session const *session_;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */

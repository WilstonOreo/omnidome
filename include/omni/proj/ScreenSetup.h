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

namespace omni {
  class Session;

  namespace proj {
    class Tuning;

    class TileIndex {
    public:
      TileIndex(int row_ = 0, int column_ = 0);

      int row() const;
      void setRow(int);
      int column() const;
      void setColumn();

    private:
      int row_ = 0;
      int column_ = 0;
    };

    /// Tiling in Rows and Columns: for screen
    class Tiling {
    public:
      Tiling(int rows = 1, int columns = 1);

      int rows() const;
      void setRows(int) const;
      int columns() const;
      void setColumns(int) const;

      void setRowsAndColumns(int rows, int columns);

      int numberOfTiles() const;

      bool contains(TileIndex const&) const;
      QRect contentRect(QRect const& screenGeometry, TileIndex const&) const;
      QSize size(QRect const&) const;
      qreal aspectRatio(QRect const&) const;

      /** @brief Static method for getting default tiling for a screen geometry.
        **/
      static Tiling defaultTiling(QRect const&);

      /// Returns screen size that supports triple heads
      static std::vector<QSize> const& screenResolutions();

      inline static constexpr int maxRows() { return 8; }
      inline static constexpr int maxColumns() { return 8; }

    private:
      int rows_ = 1;
      int columns_ = 1;
    };

    /// Singleton class which manages the current screen setup
    class ScreenSetup : public QObject {
        Q_OBJECT
      public:
        ScreenSetup(Session const *_session);

        /**@brief Returns the screen geometry for a screen.
           @detail If screen is null, the geometry for the virtual screen is returned
         **/
        QRect screenGeometry(QScreen const*) const;

        /// Return standard screen, where omnidome has to be placed
        static QScreen const          * standardScreen();

        /// Return const pointer to session
        Session const                 * session() const;

        /**@brief Returns the default tiling for a screen.
           @detail If screen is null, the default tiling for the
                   virtual screen is returned.
         **/
        static Tiling   defaultTiling(QScreen const *);

        Tiling tiling(QScreen const*) const;
        void setTiling(QScreen const*, Tiling const&);

        static QRect desktopRect(
          bool _excludeStandardScreen = true);

        /// Virtual desktop rect contains all non-assigned tunings
        QRect virtualScreenRect() const;

        /// Returns combined desktop and virtual desktop rect
        QRect combinedDesktopRect() const;

        /// Returns true if no tuning is assigned to a screen
        bool  noTuningsAssigned() const;

        bool  noTuningsAssigned(QScreen const *) const;

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
        Tiling addDefaultTilingForScreen(QScreen* screen);
        Session const *session_;
        std::map<QScreen const*,Tiling> screenTilings_;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */

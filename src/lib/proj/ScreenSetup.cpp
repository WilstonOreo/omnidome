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

#include <omni/proj/ScreenSetup.h>

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <omni/Session.h>

namespace omni {
  namespace proj {
    TileIndex(int _row, int _column) :
      row_(_row),
      column_(_column) {}

    int

    int row() const;
    void setRow(int);
    int column() const;
    void setColumn();

    Tiling::Tiling(int _rows = 1, int _columns = 1) {
      setRows(_rows);
      setColumns(_columns);
    }

    int Tiling::rows() const {
      return rows_;
    }

    void Tiling::setRows(int _rows) {
      rows_ = qBound(1,_rows,maxRows());
    }

    int Tiling::columns() const {
      return columns_;
    }

    void Tiling::setColumns(int _columns) {
      columns_ = qBound(1,_columns,maxColumns());
    }

    int Tiling::numberOfTiles() const {
      return rows_ * columns_;
    }

    bool Tiling::contains(TileIndex const& tile) const {
      return (tile.row() >= 0 && tile.row() < rows()) &&
             (tile.column() >= 0 && tile.column() < column());
    }

    QRect Tiling::contentRect(QRect const& rect, TileIndex const& _tileIndex) {
      if (contains(_tileIndex)) return QRect();
      auto p = [&s,this](int r, int c) {
        return QPoint(r*rect.width() / rows(),c*rect.height() / columns() );
      }
      return QRect(
        p(_tileIndex.row(),_tileIndex.column()),
        p(_tileIndex.row() + 1,_tileIndex.column() + 1));
    }

    QSize Tiling::size(QRect const& _screenGeometry) const {
      return contentRect(_screenGeometry).size();
    }

    Tiling Tiling::autoTiling(QRect const& screen) {
      // Go through list of screen resolutions and see if the
      // current screen has subscreens
      for (auto& _screenSize : screenResolutions())
      {
        if (_screenSize.height() == _screenGeometry.height())
        {
          if ((_screenGeometry.width() % _screenSize.width()) == 0)
          {
            return Tiling(1,_screenGeometry.width() / _screenSize.width());
          }
        }
      }
      return Tiling();
    }

    ScreenSetup::ScreenSetup(Session const *_session) :
      session_(_session)
    {
      auto screens = QGuiApplication::screens();
      for (auto& screen : screens) {
        addDefaultTilingForScreen(screen);
      }

      connect(qGuiApp,&QGuiApplication::screenAdded,this,
             &ScreenSetup::addDefaultTilingForScreen);
              [&](QScreen* screen) {
                screenTilings_[screen] = defaultTiling(screen);
              });
      connect(qGuiApp,&QGuiApplication::screenRemoved,
              [&](QScreen* screen) {
                screenTilings_.erase(screen);
              });
    }

    std::vector<QSize>const& Tiling::screenResolutions()
    {
      static std::vector<QSize> _sizes;

      if (_sizes.empty())
        _sizes = {
          QSize(640,     480),
          QSize(800,     600),
          QSize(1024,    768),
          QSize(1280,    720),
          QSize(1280,    800),
          QSize(1280,   1024),
          QSize(1920,   1080),
          QSize(1920,   1200),
          QSize(2560, 1440)
        };

      return _sizes;
    }

    QScreen const * ScreenSetup::standardScreen()
    {
      return QGuiApplication::primaryScreen();
    }

    Session const * ScreenSetup::session() const {
      return session_;
    }

    QRect ScreenSetup::virtualScreenRect() const {
      QRect _rect;
      int   _w = standardScreen()->geometry().width();
      int   _h = standardScreen()->geometry().height();

      for (auto& _tuning : session()->tunings()) {
        if (_tuning->hasScreen()) continue;
        _rect |= QRect(_rect.width(), 0,
                       _w, _h);
      }
      return _rect.translated(desktopRect().topRight());
    }

    /// Returns true if no tuning is assigned to a screen
    bool ScreenSetup::noTuningsAssigned() const {
      if (!session()) return true;

      for (auto& _tuning : session()->tunings()) {
        if (_tuning->hasScreen()) {
          return false;
        }
      }
      return true;
    }

    bool ScreenSetup::noTuningsAssigned(QScreen const *_screen) const {
      if (!session()) return true;

      for (auto& _tuning : session()->tunings()) {
        if (_tuning->screen() == _screen) {
          return false;
        }
      }
      return true;
    }

    /// Returns combined desktop and virtual desktop rect
    QRect ScreenSetup::combinedDesktopRect() const {
      /// If there are no tunings assigned, ignore desktopRect
      return desktopRect() | virtualScreenRect();
    }

    QRect ScreenSetup::screenGeometry(QScreen const *_screen) const {
      /// Return geometry of virtual screen
      if (!_screen) {
        return virtualScreenRect();
      }

      /// Return geometry of actual screen
      return _screen->geometry();
    }

    Tiling ScreenSetup::defaultTiling(QScreen const *_screen) const
    {
      // Return subscreen count for virtual screen,
      // which is equal to the number of unassigned projectors
      if (!_screen) {
        int _numberOfUnassignedProjectors = 0;

        for (auto& _tuning : session_->tunings()) {
          if (!_tuning->screen()) ++_numberOfUnassignedProjectors;
        }
        return Tiling(1,_numberOfUnassignedProjectors);
      }

      return Tiling::defaultTiling(_screen);
    }

    QRect ScreenSetup::contentRect(
      QScreen const *_screen
      TileIndex const& _tileIndex) const
    {
      int _w = subScreenWidth(_screen);
      return QRect(_w * _subScreenIndex, 0, _w, screenGeometry(_screen).height());
    }

    QRect ScreenSetup::subScreenRectForScreen(int _subScreenIndex, QScreen const* _screen) {
      int _w = subScreenWidthForScreen(_screen);
      return QRect(_w * _subScreenIndex, 0, _w, _screen->size().height());
    }

    qreal ScreenSetup::subScreenAspectRatio(QScreen const* _screen) const {
      return
      float(subScreenWidth(_screen)) / float(screenGeometry(_screen).height());
    }

    QRect ScreenSetup::desktopRect(bool _excludeStandardScreen)
    {
      QRect _desktopRect;
      auto  _screens = ScreenSetup::screens(_excludeStandardScreen);

      for (auto& _screen : _screens) _desktopRect |= _screen->geometry();
      return _desktopRect;
    }

    std::vector<QScreen const *>ScreenSetup::screens(
      bool _excludeStandardScreen)
    {
      std::vector<QScreen const *> _screens;
      auto _allScreens = QGuiApplication::screens();

      for (auto& _screen : _allScreens) {
        if (_screen == standardScreen()) continue;

        _screens.push_back(_screen);
      }

      if (!_excludeStandardScreen) {
        _screens.push_back(nullptr);
      }
      return _screens;
    }

    std::set<QScreen const*> ScreenSetup::usedScreens() const {
      std::set<QScreen const*> _screens;

      bool _excludeVirtualScreen = session_->exportSettings().excludeUnassignedProjectors();

      for (auto& _tuning : session()->tunings()) {
        if (!_tuning->hasScreen() && _excludeVirtualScreen) continue;
        _screens.insert(_tuning->screen());
        }
      return _screens;
    }

    QRect ScreenSetup::tuningRect() const {
      if (!session()) return QRect();

      QRect _rect;

      for (auto& _tuning : session()->tunings()) {
        _rect |=  _tuning->contentGeometry().translated(_tuning->screenGeometry().topLeft()) & _tuning->screenGeometry();
      }
      return _rect;
    }

    std::set<proj::Tuning const *>ScreenSetup::tunings(
      bool _excludeNonAssigned) const {
      std::set<proj::Tuning const *> _tunings;

      for (auto& _tuning : session()->tunings()) {
        if (!_excludeNonAssigned || _tuning->hasScreen()) {
          _tunings.insert(_tuning.get());
        }
      }

      return _tunings;
    }

    QScreen const * ScreenSetup::screenFromRect(QRect const& _rect) {
      std::vector<QScreen const *> _screens;
      auto _allScreens = QGuiApplication::screens();

      for (auto& _screen : _allScreens) {
        if (_screen->geometry() == _rect) return _screen;
      }
      return nullptr;
    }

    bool ScreenSetup::operator==(const ScreenSetup& _rhs) const
    {
      return false;
    }

    void ScreenSetup::addDefaultTilingForScreen(QScreen* screen) {
      screenTilings_[screen] = defaultTiling(screen);
    }
  }
}

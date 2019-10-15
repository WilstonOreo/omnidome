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
#include <QGuiApplication>
#include <omni/Session.h>

namespace omni {
  namespace proj {
    ScreenSetup::ScreenSetup(Session const *_session) :
      session_(_session)
    {}

    std::vector<QSize>const& ScreenSetup::screenResolutions()
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
          QSize(2560,   1440)
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

      /*QRect _desktopRect;
         auto  _screens = ScreenSetup::screens();
         for (auto& _screen : _screens) {
          if (noTuningsAssigned(_screen)) continue;
          _desktopRect |= _screen->geometry();
         }*/
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

    int ScreenSetup::subScreenCount(QScreen const *_screen) const
    {
      // Return subscreen count for virtual screen,
      // which is equal to the number of unassigned projectors
      if (!_screen) {
        int _numberOfUnassignedProjectors = 0;

        for (auto& _tuning : session_->tunings()) {
          if (!_tuning->screen()) ++_numberOfUnassignedProjectors;
        }
        return _numberOfUnassignedProjectors;
      }

      return subScreenCountForScreen(_screen);
    }

    int   ScreenSetup::subScreenCountForScreen(QScreen const * _screen) {
      // Go through list of screen resolutions and see if the
      // current screen has subscreens
      for (auto& _screenSize : screenResolutions())
      {
        if (_screenSize.height() == _screen->geometry().height())
        {
          if ((_screen->geometry().width() % _screenSize.width()) == 0)
          {
            return _screen->geometry().width() / _screenSize.width();
          }
        }
      }
      return 1;
    }

    int ScreenSetup::subScreenWidth(QScreen const *_screen) const
    {
      return screenGeometry(_screen).width() / subScreenCount(_screen);
    }

    int ScreenSetup::subScreenWidthForScreen(QScreen const *_screen)
    {
      return _screen->geometry().width() / subScreenCountForScreen(_screen);
    }

    QRect ScreenSetup::subScreenRect(
      int _subScreenIndex,
      QScreen const *_screen) const
    {
      int _w = subScreenWidth(_screen);
      return QRect(_w * _subScreenIndex, 0, _w, screenGeometry(_screen).height());
    }

    QRect ScreenSetup::subScreenRectForScreen(int _subScreenIndex, QScreen const* _screen) {
      int _w = subScreenWidthForScreen(_screen);
      return QRect(_w * _subScreenIndex, 0, _w, _screen->size().height());
    }

    qreal ScreenSetup::subScreenAspectRatio(QScreen const* _screen) const {
      return float(subScreenWidth(_screen)) / float(screenGeometry(_screen).height());
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
  }
}

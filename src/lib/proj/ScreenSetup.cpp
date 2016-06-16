/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
    ScreenSetup::ScreenSetup(Session const& _session) :
      session_(_session)
    {}

    QScreen const * ScreenSetup::standardScreen()
    {
      return QGuiApplication::primaryScreen();
    }

    Session const& ScreenSetup::session() const {
      return session_;
    }

    ScreenMultiplex ScreenSetup::screenMultiplex(QScreen const* _screen) const { 
      return multiplex_.count(_screen) > 0 ?
        multiplex_.at(_screen) :
        ScreenMultiplex();
    }

    void ScreenSetup::setScreenMultiplex(
        QScreen const* _screen, 
        ScreenMultiplex const& _multiplex) {
      //multiplex_[_screen] = _multiplex;
    }

    float ScreenSetup::subScreenAspectRatio(QScreen const* _screen, int _index) const {
      if (!_screen) {
        int   _w = standardScreen()->geometry().width();
        int   _h = standardScreen()->geometry().height();
        return float(_w) / float(_h); 
      }
      
      QRect&& _rect = screenMultiplex(_screen)
        .contentGeometry(screenGeometry(_screen).size(),_index);
      return float(_rect.width()) / float(_rect.height());
    }

    QRect ScreenSetup::virtualScreenRect() const {
      QRect _rect;
      int   _w = standardScreen()->geometry().width();
      int   _h = standardScreen()->geometry().height();

      for (auto& _tuning : session().tunings()) {
        if (_tuning->hasScreen()) continue;
        _rect |= QRect(_rect.width(), 0,
                       _w, _h);
      }
      return _rect.translated(desktopRect().topRight());
    }

    /// Returns true if no tuning is assigned to a screen
    bool ScreenSetup::noTuningsAssigned() const {
      for (auto& _tuning : session().tunings()) {
        if (_tuning->hasScreen()) {
          return false;
        }
      }
      return true;
    }

    bool ScreenSetup::noTuningsAssigned(QScreen const *_screen) const {
      for (auto& _tuning : session().tunings()) {
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

      bool _excludeVirtualScreen = session_.exportSettings().excludeUnassignedProjectors();

      for (auto& _tuning : session().tunings()) {
        if (!_tuning->hasScreen() && _excludeVirtualScreen) continue;
        _screens.insert(_tuning->screen());
        }
      return _screens;
    }

    QRect ScreenSetup::tuningRect() const {
      QRect _rect;

      for (auto& _tuning : session().tunings()) {
        _rect |=  _tuning->contentGeometry().translated(_tuning->screenGeometry().topLeft()) & _tuning->screenGeometry();
      }
      return _rect;
    }

    std::set<proj::Tuning const *>ScreenSetup::tunings(
      bool _excludeNonAssigned) const {
      std::set<proj::Tuning const *> _tunings;

      for (auto& _tuning : session().tunings()) {
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

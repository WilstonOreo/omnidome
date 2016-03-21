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

#include <omni/proj/ScreenSetup.h>

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
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
                    QSize(640,  480),
                    QSize(800,  600),
                    QSize(1024, 768),
                    QSize(1280, 720),
                    QSize(1280, 800),
                    QSize(1920, 1080),
                    QSize(1920, 1200),
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

        QRect ScreenSetup::virtualDesktopRect() const {
            QRect _rect;

            for (auto& _tuning : session()->tunings()) {
                if (_tuning->hasScreen()) continue;
                qDebug() << "ScreenSetup::virtualDesktopRect " << _rect;
                _rect |= QRect(_rect.width(), 0,
                               _tuning->width(), _tuning->height());
            }
            return _rect;
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

        bool ScreenSetup::noTuningsAssigned(QScreen const* _screen) const {
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
            return desktopRect() | virtualDesktopRect().translated(desktopRect().topRight());
        }

        int ScreenSetup::subScreenCount(QScreen const *_screen)
        {
            if (!_screen)   return 0;

            // Go through list of screen resolutions and see if the
            // current screen has subscreens
            for (auto& _screenSize :screenResolutions())
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

        int ScreenSetup::subScreenWidth(QScreen const *_screen)
        {
            if (!_screen)   return 0;

            return _screen->size().width() / ScreenSetup::subScreenCount(_screen);
        }

        QRect ScreenSetup::subScreenRect(QScreen const *_screen,
                                         int            _subScreenIndex)
        {
            int _w = ScreenSetup::subScreenWidth(_screen);

            return QRect(_w * _subScreenIndex, 0, _w, _screen->size().height());
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
                if (!_excludeStandardScreen || (_screen != standardScreen())) {
                    _screens.push_back(_screen);
                }
            }
            return _screens;
        }

        QRect ScreenSetup::tuningRect(proj::Tuning const *_t) const {
            if (!session()) return QRect();

            auto _r = desktopRect();

            QRect _rect;
            int   _left  = _r.left();
            int   _right = _r.right();
            int   _posX  = _r.width();

            for (auto& _tuning : session()->tunings()) {
                if (_tuning->hasScreen()) {
                    _left  = std::min(_rect.left(), _left);
                    _right = std::max(_rect.right(), _right);

                    if (_tuning.get() == _t) {
                        _rect = subScreenRect(
                            _tuning->screen(), _tuning->subScreenIndex());
                    }
                }
            }

            for (auto& _tuning : session()->tunings()) {
                if (_tuning->hasScreen()) {
                    continue;
                }

                if (_tuning.get() == _t) {
                    _rect = QRect(_posX, 0, _tuning->width(), _tuning->height());
                }
                _posX += _tuning->width();
            }

            return _rect;
        }

        QRect ScreenSetup::tuningRect() const {
            if (!session()) return QRect();

            QRect _rect;

            for (auto& _tuning : session()->tunings()) {
                _rect |= tuningRect(_tuning.get());
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

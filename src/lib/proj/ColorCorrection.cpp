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

#include <omni/proj/ColorCorrection.h>

namespace omni {
    namespace proj {
        bool ColorCorrection::isUsed() const {
            return isUsed_;
        }

        void ColorCorrection::setUsed(bool _isUsed) {
            isUsed_ = _isUsed;
        }

        ChannelCorrection& ColorCorrection::all() {
            return all_;
        }

        ChannelCorrection const& ColorCorrection::all() const {
            return all_;
        }

        void ColorCorrection::setAll(ChannelCorrection const& _all) {
            all_ = _all;
        }

        ChannelCorrection& ColorCorrection::red() {
            return red_;
        }

        ChannelCorrection const& ColorCorrection::red() const {
            return red_;
        }

        void ColorCorrection::setRed(ChannelCorrection const& _red) {
            red_ = _red;
        }

        ChannelCorrection& ColorCorrection::green() {
            return green_;
        }

        ChannelCorrection const& ColorCorrection::green() const {
            return green_;
        }

        void ColorCorrection::setGreen(ChannelCorrection const& _green) {
            green_ = _green;
        }

        ChannelCorrection& ColorCorrection::blue() {
            return blue_;
        }

        ChannelCorrection const& ColorCorrection::blue() const {
            return blue_;
        }

        void ColorCorrection::setBlue(ChannelCorrection const& _blue) {
            blue_ = _blue;
        }

        QColor ColorCorrection::corrected(QColor _color) const {
            qreal _r = all_.corrected(red_.corrected(_color.redF()));
            qreal _g = all_.corrected(green_.corrected(_color.greenF()));
            qreal _b = all_.corrected(blue_.corrected(_color.blueF()));

            return QColor(
                qBound(0, int(_r * 255), 255),
                qBound(0, int(_g * 255), 255),
                qBound(0, int(_b * 255), 255));
        }

        ChannelCorrection* ColorCorrection::correction(Channel _channel) {
            switch (_channel) {
                default:
                case Channel::ALL: return &all_;
                case Channel::RED: return &red_;
                case Channel::GREEN: return &green_;
                case Channel::BLUE: return &blue_;
            }
            return nullptr;
        }

        ChannelCorrection const* ColorCorrection::correction(Channel _channel) const {
            switch (_channel) {
                default:
                case Channel::ALL: return &all_;
                case Channel::RED: return &red_;
                case Channel::GREEN: return &green_;
                case Channel::BLUE: return &blue_;
            }
            return nullptr;
        }

        QColor ColorCorrection::channelColor(Channel _channel) {
            switch (_channel) {
                default:
                case Channel::ALL: return Qt::white; break;
                case Channel::RED: return Qt::red; break;
                case Channel::GREEN: return Qt::green; break;
                case Channel::BLUE: return Qt::blue; break;
            }
            return Qt::transparent;
        }
    }
}

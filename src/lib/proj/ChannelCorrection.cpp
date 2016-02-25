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

#include <cmath>
#include <omni/proj/ChannelCorrection.h>

namespace omni {
    namespace proj {
        qreal ChannelCorrection::gamma() const {
            return gamma_;
        }

        void ChannelCorrection::setGamma(qreal _gamma) {
            gamma_ = qBound(-1.0, _gamma, 1.0);
        }

        qreal ChannelCorrection::brightness() const {
            return brightness_;
        }

        qreal ChannelCorrection::contrast(qreal v) const {
            return ((v - 0.5) * std::max(contrast() + 1.0, 0.0)) + 0.5;
        }

        qreal ChannelCorrection::brightness(qreal v) const {
        	return v + brightness();
        }

        qreal ChannelCorrection::gamma(qreal v) const {
            qreal g = 0.0;
            if (gamma() >= 0.0) {
            	g = 1.0 / (1.0 + gamma());
            } else {
            	g = (1.0 - gamma());
            }
            return std::pow(v,g*g*g*g);
        }

        void ChannelCorrection::setBrightness(qreal _brightness) {
            brightness_ = qBound(-1.0, _brightness, 1.0);
        }

        qreal ChannelCorrection::contrast() const {
            return contrast_;
        }

        void ChannelCorrection::setContrast(qreal _contrast) {
            brightness_ = qBound(-1.0, _contrast, 1.0);
        }

        qreal ChannelCorrection::corrected(qreal _value) const {
            return brightness(contrast(gamma(_value)));
        }
    }
}

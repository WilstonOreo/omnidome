/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
#include <omni/util.h>

namespace omni {
    namespace proj {
        ChannelCorrection(QObject* parent) : QObject(parent) {

        }

        ChannelCorrection::~ChannelCorrection() {

        }

        qreal ChannelCorrection::contrastCorrected(qreal v) const {
            qreal _c = (contrast() <= 0.0) ?
                contrast()*multiplier() + 1.0 :
                (1.0 / (1.0 - contrast()*multiplier()));
            return ((v - 0.5) * std::max(_c, 0.0)) + 0.5;
        }

        qreal ChannelCorrection::brightnessCorrected(qreal v) const {
        	return std::max(v + brightness() * multiplier(),0.0);
        }

        qreal ChannelCorrection::gammaCorrected(qreal v) const {
          qreal g = 0.0;
          if (gamma() >= 0.0) {
            g = 1.0 / (1.0 + gamma() * multiplier());
          } else {
          	g = (1.0 - gamma() * multiplier());
          }
          return std::pow(v,g*g*g*g);
        }

        qreal ChannelCorrection::corrected(qreal _value) const {
            return brightness(contrast(gamma(_value)));
        }

        OMNI_DEFINE_PROPERTY_REAL_BOUND(ChannelCorrection,contrast,setContrast,-1.0,1.0);
        OMNI_DEFINE_PROPERTY_REAL_BOUND(ChannelCorrection,brightness,setBrightness,-1.0,1.0);
        OMNI_DEFINE_PROPERTY_REAL_BOUND(ChannelCorrection,gamma,setGamma,-1.0,1.0);
        OMNI_DEFINE_PROPERTY_REAL_BOUND(ChannelCorrection,multiplier,setMultiplier,0.0,1.0);
    }
}

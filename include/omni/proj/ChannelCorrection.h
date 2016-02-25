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
#ifndef OMNI_PROJ_CHANNELCORRECTION_H_
#define OMNI_PROJ_CHANNELCORRECTION_H_

#include <qglobal.h>

namespace omni {
    namespace proj {
        /// Brightness, contrast and gamma correction for a single color channel
        class ChannelCorrection {
        public:
            /// Return gamma value
            qreal gamma() const;

            /// Calculate gamma for given value
            qreal gamma(qreal v) const;

            /// Set new gamma value, value is clamped between -1 and 1
            void setGamma(qreal);

            /// Return brightness value
            qreal brightness() const;

            /// Calculate brightness for given value
            qreal brightness(qreal v) const;

            /// Set new brightness value, value is clamped between -1 and 1
            void setBrightness(qreal);

            /// Return contrast value
            qreal contrast() const;

            /// Calculate contrast for given value
            qreal contrast(qreal v) const;

            /// Set new contrast value, value is clamped between -1 and 1
            void setContrast(qreal);

            /// Return corrected value, value is NOT clamped
            qreal corrected(qreal _value) const;

        private:
            // Gamma between -1 and 1
            qreal gamma_ = 0.0;

            /// Brightness between -1 and 1
            qreal brightness_ = 0.0;

            /// Contrast between -1 and 1
            qreal contrast_ = 0.0;
        };
    }
}

#endif /* OMNI_PROJ_CHANNELCORRECTION_H_ */

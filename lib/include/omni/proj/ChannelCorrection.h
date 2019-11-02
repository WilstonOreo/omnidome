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
#ifndef OMNI_PROJ_CHANNELCORRECTION_H_
#define OMNI_PROJ_CHANNELCORRECTION_H_

#include <omni/global.h>

namespace omni {
  namespace proj {
    /// Brightness, contrast and gamma correction for a single color channel
    class OMNI_EXPORT ChannelCorrection {
      public:
        /// Return gamma value
        double      gamma() const;

        /// Calculate gamma for given value
        double      gamma(double v) const;

        /// Set new gamma value, value is clamped between -1 and 1
        void        setGamma(double);

        /// Return brightness value
        double      brightness() const;

        /// Calculate brightness for given value
        double      brightness(double v) const;

        /// Set new brightness value, value is clamped between -1 and 1
        void        setBrightness(double);

        /// Return contrast value
        double      contrast() const;

        /// Calculate contrast for given value
        double      contrast(double v) const;

        /// Set new contrast value, value is clamped between -1 and 1
        void        setContrast(double);

        /// Value contrast, brightness and gamma are multiplied
        double      multiplier() const;

        /// Set new multiplier value
        void        setMultiplier(double _multiplier);

        /// Return corrected value, value is NOT clamped
        double      corrected(double _value) const;

        /// Test for equality
        friend bool operator==(ChannelCorrection const&,
                               ChannelCorrection const&);

      private:
        // Gamma between -1 and 1
        double gamma_ = 0.0;

        /// Brightness between -1 and 1
        double brightness_ = 0.0;

        /// Contrast between -1 and 1
        double contrast_ = 0.0;

        /// Multiplier between 0.0 and 1.0
        double multiplier_ = 0.5;
    };
  }
}

QDataStream& operator>>(QDataStream&,
                        omni::proj::ChannelCorrection&);
QDataStream& operator<<(QDataStream&,
                        omni::proj::ChannelCorrection const&);

#endif /* OMNI_PROJ_CHANNELCORRECTION_H_ */

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

#include <omni/Serializer.h>
#include <omni/property.h>

namespace omni {
  namespace proj {
    /// Brightness, contrast and gamma correction for a single color channel
    class ChannelCorrection : public QObject, public Serializer<ChannelCorrection>  {
        Q_OBJECT
        OMNI_PROPERTY_CLAMPED(qreal,gamma,setGamma,0.0,-1.0,1.0)
        OMNI_PROPERTY_CLAMPED(qreal,brightness,setBrightness,0.0,-1.0,1.0)
        OMNI_PROPERTY_CLAMPED(qreal,contrast,setContrast,0.0,-1.0,1.0)
        OMNI_PROPERTY_CLAMPED(qreal,multiplier,setMultiplier,0.5,0.0,1.0)
      public:
        ChannelCorrection(QObject* = nullptr);
        ~ChannelCorrection();

        /// Calculate gamma corrected value
        Q_INVOKABLE qreal gammaCorrected(qreal v) const;

        /// Calculate brightness for given value
        Q_INVOKABLE qreal      brightnessCorrected(qreal v) const;

        /// Calculate contrast for given value
        Q_INVOKABLE qreal      contrastCorrected(qreal v) const;

        /// Return corrected value, value is NOT clamped!
        Q_INVOKABLE qreal      corrected(qreal _value) const;

      signals:
        void gammaChanged();
        void brightnessChanged();
        void contrastChanged();
        void multiplierChanged();
    };
  }
}

#endif /* OMNI_PROJ_CHANNELCORRECTION_H_ */

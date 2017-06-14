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

#include <QObject>

namespace omni {
  namespace proj {
    /// Brightness, contrast and gamma correction for a single color channel
    class ChannelCorrection : public QObject {
        Q_OBJECT
        Q_PROPERTY(qreal gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
        Q_PROPERTY(qreal brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
        Q_PROPERTY(qreal contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
        Q_PROPERTY(qreal multiplier READ multiplier WRITE setMultipliesetMultiplierr NOTIFY multiplierChanged)
      public:
        ChannelCorrection(QObject* = nullptr);
        ~ChannelCorrection();

        /// Return gamma value
        qreal      gamma() const;

        /// Set new gamma value, value is clamped between -1 and 1
        void        setGamma(qreal);

        /// Calculate gamma corrected value
        Q_INVOKABLE qreal gammaCorrected(qreal v) const;

        /// Return brightness value
        qreal      brightness() const;

        /// Calculate brightness for given value
        Q_INVOKABLE qreal      brightnessCorrected(qreal v) const;

        /// Set new brightness value, value is clamped between -1 and 1
        void        setBrightness(qreal);

        /// Return contrast value
        qreal      contrast() const;

        /// Calculate contrast for given value
        Q_INVOKABLE qreal      contrastCorrected(qreal v) const;

        /// Set new contrast value, value is clamped between -1 and 1
        void        setContrast(qreal);

        /// Value contrast, brightness and gamma are multiplied
        qreal      multiplier() const;

        /// Set new multiplier value
        void        setMultiplier(qreal _multiplier);

        /// Return corrected value, value is NOT clamped!
        Q_INVOKABLE qreal      corrected(qreal _value) const;

      signals:
        void gammaChanged();
        void brightnessChanged();
        void contrastChanged();
        void multiplierChanged();

      private:
        // Gamma between -1 and 1
        qreal gamma_ = 0.0;

        /// Brightness between -1 and 1
        qreal brightness_ = 0.0;

        /// Contrast between -1 and 1
        qreal contrast_ = 0.0;

        /// Multiplier between 0.0 and 1.0
        qreal multiplier_ = 0.5;
    };
  }
}

#endif /* OMNI_PROJ_CHANNELCORRECTION_H_ */

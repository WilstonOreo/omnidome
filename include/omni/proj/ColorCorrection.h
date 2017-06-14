/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can blueistribute it and/or modify
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
#ifndef OMNI_PROJ_COLORCORRECTION_H_
#define OMNI_PROJ_COLORCORRECTION_H_

#include <omni/proj/ChannelCorrection.h>
#include <omni/proj/Channel.h>
#include <omnic/ColorCorrectionLOT.h>
#include <QColor>
#include <omni/serialization/Serializer.h>

namespace omni {
  namespace proj {
    /// Color correction for RGB color space
    class ColorCorrection : public QObject, public Serializer<ColorCorrection> {
      Q_OBJECT
      Q_PROPERTY(bool used READ isUsed WRITE setUsed NOTIFY usedChanged)
      Q_PROPERTY(ChannelCorrection* all READ all CONSTANT)
      Q_PROPERTY(ChannelCorrection* red READ red CONSTANT)
      Q_PROPERTY(ChannelCorrection* blue READ blue CONSTANT)
      Q_PROPERTY(ChannelCorrection* green READ green CONSTANT)
    public:
        ColorCorrection(QObject* = nullptr);
        ~ColorCorrection();

        bool                     isUsed() const;
        void                     setUsed(bool _isUsed);

        /// Return channel correction for all channels
        ChannelCorrection      * all() const;

        /// Return channel correction for red channel
        ChannelCorrection      * red() const;

        /// Return channel correction for green channel
        ChannelCorrection      * green() const;

        /// Return reference to channel correction for blue channel
        ChannelCorrection      * blue() const;

        /// Return color correction for given channel
        ChannelCorrection *correction(Channel) const;

        /// Calculate color look up tables for omnidome calibration format
        omnic::ColorCorrectionLOT calculateLookUpTable(uint32_t _quantization = 1024) const;

        /// Return corrected color
        Q_INVOKABLE QColor corrected(QColor) const;

        /// Return QColor for channel
        Q_INVOKABLE static QColor channelColor(Channel);
      signals:
        void usedChanged();

      private:
        bool isUsed_ = true;
        ChannelCorrection* all_;
        ChannelCorrection* red_;
        ChannelCorrection* green_;
        ChannelCorrection* blue_;
    };
  }

  using proj::ColorCorrection;
}

#endif /* OMNI_PROJ_COLORCORRECTION_H_ */

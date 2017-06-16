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

namespace omni {
  namespace proj {
    /// Color correction for RGB color space
    class ColorCorrection : public QObject, public Serializer<ColorCorrection> {
      Q_OBJECT
      OMNI_PROPERTY_RW_DEFAULT(bool,isUsed,setUsed,true)
      OMNI_PROPERTY_OBJ(ChannelCorrection,all)
      OMNI_PROPERTY_OBJ(ChannelCorrection,red)
      OMNI_PROPERTY_OBJ(ChannelCorrection,blue)
      OMNI_PROPERTY_OBJ(ChannelCorrection,green)
    public:
        ColorCorrection(QObject* = nullptr);
        ~ColorCorrection();

        /// Return color correction for given channel
        ChannelCorrection *correction(Channel) const;

        /// Calculate color look up tables for omnidome calibration format
        omnic::ColorCorrectionLOT calculateLookUpTable(uint32_t _quantization = 1024) const;

        /// Return corrected color
        Q_INVOKABLE QColor corrected(QColor) const;

        /// Return QColor for channel
        Q_INVOKABLE static QColor channelColor(Channel);
    signals:
        void isUsedChanged();
    };
  }

  using proj::ColorCorrection;
}

#endif /* OMNI_PROJ_COLORCORRECTION_H_ */

/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#include <QColor>

namespace omni {
    namespace proj {
        /// Color correction for RGB color space
        class ColorCorrection {
        public:
            bool isUsed() const;
            void setUsed(bool _isUsed);

            /// Return reference to channel correction for all channels
            ChannelCorrection& all();

            /// Return reference to channel correction for all channels (const)
            ChannelCorrection const& all() const;

            /// Set channel correction for all channels
            void setAll(ChannelCorrection const&);

            /// Return reference to channel correction for red channel
            ChannelCorrection& red();

            /// Return reference to channel correction for red channel
            ChannelCorrection const& red() const;

            /// Set channel correction for red channel
            void setRed(ChannelCorrection const&);

            /// Return reference to channel correction for green channel
            ChannelCorrection& green();

            /// Return reference to channel correction for green channel
            ChannelCorrection const& green() const;

            /// Set channel correction for green channel
            void setGreen(ChannelCorrection const&);

            /// Return reference to channel correction for blue channel
            ChannelCorrection& blue();

            /// Return reference to channel correction for blue channel
            ChannelCorrection const& blue() const;

            /// Set channel correction for blue channel
            void setBlue(ChannelCorrection const&);

            /// Return corrected color
            QColor corrected(QColor) const;

        private:
            bool isUsed_ = true;
            ChannelCorrection all_, red_, green_, blue_;
        };
    }
}

#endif /* OMNI_PROJ_COLORCORRECTION_H_ */

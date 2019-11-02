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

#ifndef OMNI_PROJ_CALIBRATION_H_
#define OMNI_PROJ_CALIBRATION_H_

#include <QImage>
#include <QRect>
#include <QSize>
#include <omni/global.h>
#include <omnic/CalibratedProjector.h>
#include <omnic/gl/Framebuffer.h>
#include <omni/proj/ColorCorrection.h>
#include <omni/visual/ContextBoundPtr.h>

namespace omni {
  namespace proj {
    class Tuning;

    /**@brief Calibration generated from a tuning
       @detail Calibration has output mode with either UVW or TEXCOORDS
     **/
    class OMNI_EXPORT Calibration {
      public:
        typedef omnic::PixelData pixeldata_type;

        /// Create empty calibration from with optional output mode
        Calibration();

        /// Create and render calibration from tuning
        Calibration(Tuning const&);

        /// Render calibration from tuning
        void render(Tuning const&);

        /// Render to framebuffer object
        void render(Tuning const&, ContextBoundPtr<omnic::gl::FramebufferRGBA16>&);

        /// Render to image
        QImage                 toImage() const;

        /// Render to preview image
        QImage                 toPreviewImage() const;

        /// Return pixeldata
        pixeldata_type const   & pixelData() const;

        /// Return screen geometry rectangle
        QRect const          & screenGeometry() const;

        /// Return output geometry rectangle
        QRect const          & contentGeometry() const;

        /// Return actual size of rendered output
        QSize                  renderSize() const;

        /// Set new size for renderer output. Resets calibration.
        void                   setRenderSize(QSize const&);

        /// Return color correction
        ColorCorrection const& colorCorrection() const;

        /// Return true if screen is virtual
        bool                   virtualScreen() const;

        omnic::CalibratedProjector calibratedProjector() const;

      private:
        template<typename OPERATION>
        static void bufferToImage(pixeldata_type const& _buffer,
                                  QImage& _image,
                                  OPERATION _f);

        /// Encode upper 8 bit of buffer into image
        void getUpper8bit(QImage& _image) const;

        /// Encode lower 8 bit of buffer into image
        void getLower8bit(QImage& _image) const;

        /// Encode alpha mask into selected channel
        void encodeAlphaMask(QImage & _image,
                          Channel = Channel::ALL) const;

        /// Encode color correction into selected image channel
        void encodeColorCorrection(QImage&, Channel) const;

        bool  virtualScreen_ = true;
        QRect screenGeometry_;
        QRect contentGeometry_;
        ColorCorrection colorCorrection_;
        pixeldata_type data_;
    };
  }
}

#endif /* OMNI_PROJ_CALIBRATION_H_ */

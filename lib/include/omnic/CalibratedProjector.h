/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OMNIC_CALIBRATEDPROJECTOR_H_
#define OMNIC_CALIBRATEDPROJECTOR_H_

#include "Rect.h"
#include "PixelData.h"
#include "ColorCorrectionLOT.h"

namespace omnic {
  /// Data structure for calibrated projector
  class CalibratedProjector {
  public:
    /// Return flag if projector has a virtual screen assigned
    inline bool virtualScreen() const {
      return screenInfo_ & 1; // Virtual screen is first bit
    }

    /// Set virtual screen
    inline void setVirtualScreen(bool _virtualScreen) {
      screenInfo_ &= 0xFFFFFFFE;
      screenInfo_ |= _virtualScreen; 
    }

    /// Rectangle representing the screen geometry
    inline Rect const& screenGeometry() const {
      return screenGeometry_;
    }
    
    /// Set screen geometry
    inline void setScreenGeometry(Rect const& _screenGeometry) {
      screenGeometry_ = _screenGeometry;
    }
    
    /// Rectangle representing the content placement inside the screen
    inline Rect const& contentGeometry() const {
      return contentGeometry_;
    }
    
    /// Set Content  geometry
    inline void setContentGeometry(Rect const& _contentGeometry) {
      contentGeometry_ = _contentGeometry;
    }

    /// Return color correction look up table
    inline ColorCorrectionLOT const& colorCorrection() const {
      return colorCorrection_;
    }

    /// Set new color correction look up table
    inline void setColorCorrection(ColorCorrectionLOT const& _colorCorrection) {
      colorCorrection_ = _colorCorrection;
    }

    /// Return pixel data
    inline PixelData const& pixelData() const {
      return pixelData_;
    }
    
    /// Set pixel data
    inline void setPixelData(PixelData const& _pixelData) {
      pixelData_ = _pixelData;
    }

    /// Load from stream
    template<typename STREAM>
    void load(STREAM& _is, Version _version = Version::current())
    {
      using namespace util;
      readBinary(_is,screenInfo_);
      screenGeometry_.load(_is,_version);
      contentGeometry_.load(_is,_version);
      colorCorrection_.load(_is,_version);
      pixelData_.load(_is,_version);
    }
    
    /// Save to stream
    template<typename STREAM>
    void save(STREAM& _os, Version _version = Version::current()) const
    {
      using namespace util;
      writeBinary(_os,screenInfo_);
      screenGeometry_.save(_os,_version);
      contentGeometry_.save(_os,_version);
      colorCorrection_.save(_os,_version);
      pixelData_.save(_os,_version);
    }

    /// Test for equality 
    inline friend bool operator==(
        CalibratedProjector const& _lhs, 
        CalibratedProjector const& _rhs) {
      return 
        (_lhs.screenInfo_ == _rhs.screenInfo_) &&
        (_lhs.screenGeometry_ == _rhs.screenGeometry_) &&
        (_lhs.contentGeometry_ == _lhs.contentGeometry_) &&
        (_lhs.colorCorrection_ == _rhs.colorCorrection_) &&
        (_lhs.pixelData_ == _rhs.pixelData_)
        ;
    }
  
  private:
    uint32_t screenInfo_ = 0;
    Rect screenGeometry_;
    Rect contentGeometry_;
    ColorCorrectionLOT colorCorrection_;
    PixelData pixelData_;
  };
}

#endif /* OMNIC_CALIBRATEDPROJECTOR_H_ */



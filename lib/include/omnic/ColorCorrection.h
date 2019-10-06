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
#ifndef OMNIC_COLORCORRECTION_H_
#define OMNIC_COLORCORRECTION_H_

#include "ChannelCorrection.h"
#include "ColorCorrectionLOT.h"

namespace omnic {
  /// Color Correction with correction for each channel and overall color correction
  struct ColorCorrection {

    /// Load from stream
    template<typename T>
    inline void load(STREAM& _is, Version _version = Version::latest()) {
      red_.load(_is,_version);
      green_.load(_is,_version);
      blue_.load(_is,_version);
      all_.load(_is,_version);
    }

    /// Return correction for red channel
    inline ChannelCorrection const& red() const {
      return red_;
    }

    /// Return correction for green channel
    inline ChannelCorrection const& green() const {
      return green_;
    }
    
    /// Return correction for blue channel
    inline ChannelCorrection const& blue() const {
      return blue_;
    }
    
    /// Return correction for all channel
    inline ChannelCorrection const& all() const {
      return all_;
    }

    inline ColorCorrectionLOT calculateLookUpTable(uint32_t _quantization = 1024) {
 
      ColorCorrectionLOT _lot;
      std::vector<ColorCorrectionInfo> _data(_quantization);
      for (size_t i = 0; i < _data.size(); ++i) {
        float _red = red_.corrected(i / float(_quantization));
        float _green = green_.corrected(i / float(_quantization));
        float _blue = blue_.corrected(i / float(_quantization));
        float _all = all_.corrected(i / float(_quantization));
        _data[i] = ColorCorrectionInfo(_red,_green,_blue,_all);
      }
      _lot.data_ = _data;
      return _lot;
    }

  private:
    ChannelCorrection red_, green_, blue_, all_;
  };
}

#endif /* OMNIC_COLORCORRECTION_H_ */

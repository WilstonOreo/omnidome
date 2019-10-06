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
#ifndef OMNIC_CHANNELCORRECTION_H_
#define OMNIC_CHANNELCORRECTION_H_

#include "util.h"

namespace omnic
{
  /// Correction for a single color channel
  struct ChannelCorrection
  {
  public:
    /// Gamma correction from -1.0 to 1.0
    inline float gamma() const
    {
      return gamma_;
    }

    /// Brightness correction from -1.0 to 1.0
    inline float brightness() const
    {
      return brightness_;
    }

    /// Contrast correction from -1.0 to 1.0
    inline float contrast() const
    {
      return contrast_;
    }

    /// Correction multiplier from 0.0 to 1.0
    inline float multiplier() const
    {
      return multiplier_;
    }

    /// Load from stream
    template<typename STREAM>
    inline void load(STREAM& _is, Version _version = Version::latest())
    {
      using namespace util;
      readBinary(_is,gamma_);
      keepInRange(gamma_);
      readBinary(_is,brightness_);
      keepInRange(brightness_);
      readBinary(_is,contrast_);
      keepInRange(contrast_);
      readBinary(_is,multiplier_);
      keepInRange(multiplier_,0.0f,1.0f);
    }

    inline float brightness(float v, float brightness_value)
    {
      return std::max(v + brightness_value,0.0f);
    }

    /// Calculate contrast corrected value
    inline float contrast(float v, float contrast_value)
    {
      float _c = (contrast_value <= 0.0) ?
                 contrast_value + 1.0 :
                 (1.0 / (1.0 - contrast_value));
      return (v - 0.5) * std::max(_c, 0.0f) + 0.5;
    }

    /// Calculate gamma corrected value
    inline float gamma(float v, float gamma_value)
    {
      float g = 0.0;
      if (gamma_value >= 0.0)
      {
        g = 1.0 / (1.0 + gamma_value);
      }
      else
      {
        g = (1.0 - gamma_value);
      }
      return pow(v,g*g*g*g);
    }

    /// Calculate corrected pixel value from given pixel value
    inline float corrected(float v)
    {
      return brightness(contrast(gamma(v,
                                       gamma() * multiplier()),
                                 contrast() * multiplier()),
                        brightness ()* multiplier());
    }

    /// Appends lookup table data to given input vector of float
    template<typename T>
    inline void calculateLookUpTable(T& _t, uint32_t _quantization = 256) {
      size_t _start = _t.size();
      _t.resize(_t.size() + _quantization);
      size_t _end = _t.size();

      for (size_t i = 0; i < _quantization; ++i) {
        _t[i + _start] = corrected(i / float(_quantization));
      }
    }

  private:
    /// Clamps value of a member in min/max range
    inline static void keepInRange(float& _f, float _min = -1.0, float _max = 1.0)
    {
      _f = util::clamp(_f,_min,_max);
    }

    float gamma_ = 0.0;
    float brightness_ = 0.0;
    float contrast_ = 0.0;
    float multiplier_ = 0.0;
  }
};

}

#endif /* OMNIC_CHANNELCORRECTION_H_ */

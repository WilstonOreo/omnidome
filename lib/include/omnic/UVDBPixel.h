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
#ifndef OMNIC_UVDBPIXEL_H_
#define OMNIC_UVDBPIXEL_H_

#include <algorithm>
#include <cmath>

namespace omnic {
  /**@brief Calibrated pixel as UVW texture coordinates and blend mask value
   **/
  struct UVDBPixel {
    typedef uint16_t channel_type;
    UVDBPixel() {
    }

    UVDBPixel(channel_type _u, channel_type _v, channel_type _d, channel_type _b) :
      u_(_u),
      v_(_v),
      d_(_d),
      b_(_b) {
    }

    /// U Texture coordinate
    inline channel_type u() const {
      return u_;
    }

    /// V Texture coordinate
    inline channel_type v() const {
      return v_;
    }
    /// D depth information for sampling
    inline channel_type d() const {
      return d_;
    }
    /// Blend mask value
    inline channel_type b() const {
      return b_;
    }

    inline static UVDBPixel average(
        UVDBPixel const& _x00,
        UVDBPixel const& _x01,
        UVDBPixel const& _x10,
        UVDBPixel const& _x11) {
        return UVDBPixel(
          (_x00.u() + _x01.u() + _x10.u() + _x11.u()) / 4,
          (_x00.v() + _x01.v() + _x10.v() + _x11.v()) / 4,
          calculateDepthValue(_x00,_x01,_x10,_x11),
          (_x00.b() + _x01.b() + _x10.b() + _x11.b()) / 4);
    }

    /// Test for equality 
    inline friend bool operator==(
        UVDBPixel const& _lhs, 
        UVDBPixel const& _rhs) {
      return 
        (_lhs.u_ == _rhs.u_) &&
        (_lhs.v_ == _rhs.v_) &&
        (_lhs.d_ == _rhs.d_) &&
        (_lhs.b_ == _rhs.b_);
    }


  private:
    inline static channel_type calculateDepthValue(
        UVDBPixel const& _x00,
        UVDBPixel const& _x01,
        UVDBPixel const& _x10,
        UVDBPixel const& _x11) {
      return (diff(_x00,_x01)*2 + diff(_x00,_x10)*2 + diff(_x00,_x11)) / 10;
    }

    inline static int diff(
        UVDBPixel const& _a,
        UVDBPixel const& _b) {
      return
        abs(int(_a.u()) - int(_b.u())) +
        abs(int(_b.u()) - int(_b.v()));
    }

    channel_type u_,v_,d_,b_;
  };
}

#endif /* OMNIC_UVDBPIXEL_H_ */

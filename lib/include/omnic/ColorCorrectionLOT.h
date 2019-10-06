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

#ifndef OMNIC_COLORCORRECTIONLOT_H_
#define OMNIC_COLORCORRECTIONLOT_H_

#include <vector>
#include <omnic/util.h>
#include <omnic/Version.h>

namespace omnic {
  /// Color correction information
  struct ColorCorrectionInfo {
    typedef float value_type;

    ColorCorrectionInfo() {}

    ColorCorrectionInfo(
        value_type _red,
        value_type _green,
        value_type _blue,
        value_type _all) :
      red_(_red),
      green_(_green),
      blue_(_blue),
      all_(_all) {}

    /// Red value
    inline value_type red() const {
      return red_;
    }

    /// Green value
    inline value_type green() const {
      return green_;
    }

    /// Blue value
    inline value_type blue() const {
      return blue_;
    }
    /// All value
    inline value_type all() const {
      return all_;
    }
    
    /// Test for equality 
    friend bool operator==(
        ColorCorrectionInfo const& _lhs, 
        ColorCorrectionInfo const& _rhs) {
      return 
        (_lhs.red_ == _rhs.red_) &&
        (_lhs.green_ == _rhs.green_) &&
        (_lhs.blue_ == _rhs.blue_) &&
        (_lhs.all_ == _rhs.all_);
        ;
    }

  private:
    float red_;
    float green_;
    float blue_;
    float all_;
  };

  /// Look up table for color correction
  struct ColorCorrectionLOT {
    typedef std::vector<ColorCorrectionInfo> data_type;

    /// Maximum size of look up table
    inline static constexpr size_t maxSize() { return 65536; }
    
    /// Minimum size of look up table
    inline static constexpr size_t minSize() { return 256; }
    
    /// Default size of look up table
    inline static constexpr size_t defaultSize() { return 1024; }

    /// Default size of look up table
    inline uint32_t size() const {
      return data_.size();
    }

    /// Return pointer to data
    inline const void* ptr() const {
      return (void const*)data_.data();
    }

    /// Return data as vector
    inline std::vector<ColorCorrectionInfo> const& data() const {
      return data_;
    }

    /// Set data from input container
    template<typename T>
    void setData(T const& _data) {
      data_ = _data;
    }

    /// Save lookup table data to stream
    template<typename STREAM>
    void save(STREAM& _os, Version = Version::current()) const {
      util::writeBinary(_os,uint32_t(data_.size()));
      _os.write((char const*)(data_.data()),data_.size() * sizeof(ColorCorrectionInfo));
    }

    /// Load lookup table data from stream
    template<typename STREAM>
    void load(STREAM& _is, Version = Version::current()) {
      using namespace util;
      uint32_t _size = 0;
      readBinary(_is,_size);
      OMNIC_ASSERT(_size <= maxSize());
      data_.resize(_size);
      _is.read((char*)(data_.data()),data_.size() * sizeof(ColorCorrectionInfo));
    }
    
    /// Test for equality 
    friend bool operator==(
        ColorCorrectionLOT const& _lhs, 
        ColorCorrectionLOT const& _rhs) {
      return (_lhs.data_ == _rhs.data_);
    }

  private:
    data_type data_;
  };
}

#endif /* OMNIC_COLORCORRECTIONLOT_H_ */

/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * The MIT License (MIT)
 * Copyright (c) 2016
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef OMNIC_PIXELBUFFER_H_
#define OMNIC_PIXELBUFFER_H_

#include <vector>
#include <omnic/Version.h>
#include <omnic/UVDBPixel.h>

namespace omnic
{
  /// Pixel buffer stores matrix with calibration values foreach pixel
  struct PixelData
  {
    typedef std::vector<UVDBPixel> data_type;

    PixelData() {}

    PixelData(uint32_t _width, uint32_t _height, data_type const& _data = data_type()) : 
      width_(_width),
      height_(_height),
      data_(_data) {
      if (width_ * height_ != data_.size()) resize(_width,_height);
    }

    inline static constexpr size_t maxResolution()
    {
      return 65536;
    }

    /// Clear pixel data
    inline void clear() {
      width_ = 0;
      height_ = 0;
      data_.clear();
    }

    /// Resize to new resolution, removes old data
    inline void resize(uint32_t _width, uint32_t _height) {
      width_ = _width;
      height_ = _height;
      data_.resize(width_ * height_);
    }

    /// Buffer width
    inline uint32_t width() const
    {
      return width_;
    }

    /// Buffer height
    inline uint32_t height() const
    {
      return height_;
    }

    /// Const void pointer to data
    inline void const* ptr() const
    {
      return (void const*)data_.data();
    }

    /// Void pointer to data
    inline void* ptr()
    {
      return (void*)data_.data();
    }

    /// Return vector of stored pixel values
    inline data_type const& data() const
    {
      return data_;
    }

    /// Reduce resolution by a factor 2 in each dimension
    inline void downsample()
    {
      uint32_t _newWidth = width_ / 2;
      uint32_t _newHeight = height_ / 2;
      data_type _newData(_newWidth * _newHeight);

      for (uint32_t y = 0; y < _newHeight; ++y)
      {
        for (uint32_t x = 0; x < _newWidth; ++x)
        {
          uint32_t xx0 = x*2;
          uint32_t yy0 = y*2;
          uint32_t xx1 = xx0+1;
          uint32_t yy1 = yy0+1;

          if (yy1 >= height_)
          {
            yy1 = yy0;
          }
          if (xx1 >= width_)
          {
            xx1 = xx0;
          }

          _newData[y * _newWidth + x] = UVDBPixel::average(
                                          data_[(yy0)*width_ + xx0],
                                          data_[(yy0)*width_ + xx1],
                                          data_[(yy1)*width_ + xx0],
                                          data_[(yy1)*width_ + xx1]);
        }
      }

      data_ = _newData;
      width_ = _newWidth;
      height_ = _newHeight;
    }

    /// Downsamples the stored buffer n times
    inline void downsample(int _times)
    {
      for (int i = 0; i < _times; ++i)
      {
        downsample();
      }
    }

    /// Downsample to maximum resolution
    inline void downsample(uint32_t _maxWidth, uint32_t _maxHeight)
    {
      while (width_ >= _maxWidth && height_ >= _maxHeight)
      {
        downsample();
      }
    }

    /// Load from stream
    template<typename STREAM>
    void load(STREAM& _stream, Version = Version::current())
    {
      using namespace util;
      readBinary(_stream,width_);
      readBinary(_stream,height_);
      OMNIC_ASSERT(width_ <= maxResolution());
      OMNIC_ASSERT(height_ <= maxResolution());
      data_.resize(width_ * height_);
      _stream.read((char*)(data_.data()),data_.size() * sizeof(UVDBPixel));
    }
  
    /// Save to stream
    template<typename STREAM>
    void save(STREAM& _stream, Version = Version::current()) const {
      using namespace util;
      writeBinary(_stream,width_);
      writeBinary(_stream,height_);
      _stream.write((char const*)(data_.data()),data_.size() * sizeof(UVDBPixel));
    }

    /// Test for equality 
    inline friend bool operator==(
        PixelData const& _lhs, 
        PixelData const& _rhs) {
      return 
        (_lhs.width_ == _rhs.width_) &&
        (_lhs.height_ == _rhs.height_) &&
        (_lhs.data_ == _rhs.data_);
    }

  private:
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    data_type data_;
  };
}

#endif /* OMNIC_PIXELBUFFER_H_ */

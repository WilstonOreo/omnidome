/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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
#ifndef OMNI_BUFFER_H_
#define OMNI_BUFFER_H_

#include <cstdint>
#include <cstddef>
#include <QRect>
#include <QImage>
#include <omni/util.h>
#include <omni/PixelConverter.h>
#include <omni/serialization/container.h>

namespace omni {
  /// A buffer holds an w x h pixel array
  template<typename T>
  struct Buffer
  {
    /// Our pixel type
    typedef T pixel_type;

    /// Our data type (a dynamic array)
    typedef std::vector<pixel_type>data_type;

    /// Standard constructor
    Buffer() : width_(0), height_(0) {}

    /// Constructor with automatic resize
    Buffer(int _width, int _height) {
      resize(_width, _height);
    }

    /// Write pixel value to position (x,y)
    void put(int _x, int _y, const pixel_type& _pixel)
    {
      put(_y * width() + _x, _pixel);
    }

    /// Write pixel value to offset (= x * width + y)
    void put(size_t _offset, const pixel_type& _pixel)
    {
      data_[_offset] = _pixel;
    }

    /// Return pixel on position (x,y)
    pixel_type& operator()(int _x, int _y)
    {
      return pixel(_x, _y);
    }

    /// Return pixel on position (x,y) (const version)
    pixel_type const& operator()(int _x, int _y) const
    {
      return pixel(_x, _y);
    }

    /// Return pixel on offset (= x * width + y)
    pixel_type& operator()(size_t _offset)
    {
      return pixel(_offset);
    }

    /// Return pixel on offset (= x * width + y) (const version)
    pixel_type const& operator()(size_t _offset) const
    {
      return pixel(_offset);
    }

    /// Subscript operator for accessing elements by index
    pixel_type& operator[](size_t _offset) {
      return pixel(_offset);
    }

    /// Subscript operator for accessing elements by index (const version)
    pixel_type const& operator[](size_t _offset) const {
      return pixel(_offset);
    }

    /// Return pixel on position (x,y)
    pixel_type& pixel(int _x, int _y)
    {
      return pixel(_y * width() + _x);
    }

    Buffer cropRect(QRect const& _rect) const
    {
      Buffer _buffer(_rect.width(), _rect.height());

      for (int y = 0; y < _rect.height(); ++y)
        for (int x = 0; x < _rect.width(); ++x)
        {
          _buffer(x, y) = pixel(x + _rect.x(), y + _rect.y());
        }

      return _buffer;
    }

    /// Return pixel on position (x,y) (const version)
    pixel_type const& pixel(int _x, int _y) const
    {
      return pixel(_y * width() + _x);
    }

    /// Return pixel on offset (= x * width + y)
    pixel_type& pixel(size_t _offset) {
      return data_[_offset];
    }

    /// Return pixel on offset (= x * width + y) (const version)
    pixel_type const& pixel(size_t _offset) const {
      return data_[_offset];
    }

    /// Return width of the buffer
    int width() const
    {
      return width_;
    }

    /// Return height of the buffer
    int height() const
    {
      return height_;
    }

    /// Return const reference to data
    data_type const& data() const
    {
      return data_;
    }

    /// Clear buffer with black color
    void clear()
    {
      data_ = data_type(data_.size(), 0);
    }

    /// Clear buffer with specific pixel
    void clear(const pixel_type& _pixel)
    {
      data_ = data_type(data_.size(), _pixel);
    }

    /// Returns true if width and height are zero
    bool empty() const
    {
      return data_.empty();
    }

    /// Returns size of buffer (= width * height)
    size_t size() const {
      return data_.size();
    }

    /// Resize buffer to given width and height
    void resize(int _width, int _height)
    {
      if (_width * _height <= 0) return;

      width_  = _width;
      height_ = _height;
      data_.resize(width_ * height_);
    }

    /// Convert buffer to QImage
    QImage toQImage() const
    {
      QImage _output(width(), height(), QImage::Format_RGB32);

      for (int y = 0; y < height(); ++y)
        for (int x = 0; x < width(); ++x)
        {
          auto _p = pixel(x, y);
          _output.setPixel(x, y, convertPixel<QColor>(_p).rgb());
        }

      return _output;
    }

    /// Return void pointer to data
    void* ptr()
    {
      return static_cast<void *>(data_.data());
    }

    /// Return void pointer to data (const version)
    void const* ptr() const
    {
      return static_cast<void const*>(data_.data());
    }

    /// Write blend mask to stream
    void toStream(QDataStream& _os) const {
      serialize(_os, width_);
      serialize(_os, height_);
      serialize(_os, data_);
    }

    /// Read blend mask from stream
    void fromStream(QDataStream& _is) {
      deserialize(_is, width_);
      deserialize(_is, height_);
      deserialize(_is, data_);
    }

    /// Test for equality, buffer is ignored
    friend bool operator==(Buffer const& _lhs, Buffer const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(width_) &&
        OMNI_TEST_MEMBER_EQUAL(height_) &&
        OMNI_TEST_MEMBER_EQUAL(data_);
    }

    private:
      int       width_, height_;
      data_type data_;
  };
}

/// Deserialize buffer from stream
template<typename T>
QDataStream& operator>>(QDataStream& _is, omni::Buffer<T>& _buf) {
  _buf.fromStream(_is);
  return _is;
}

/// Serialize buffer to stream
template<typename T>
QDataStream& operator<<(QDataStream& _os, omni::Buffer<T>const& _buf) {
  _buf.toStream(_os);
  return _os;
}

#endif /* OMNI_BUFFER_H_ */

/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#ifndef OMNI_BLENDBUFFER_H_
#define OMNI_BLENDBUFFER_H_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <QRect>

class QImage;

namespace omni
{
  /// Blend buffer holds an w x h pixel array
  struct BlendBuffer
  {
    /// Our pixel type (1 unsigned byte)
    typedef uint8_t pixel_type;

    /// Our data type (a dynamic array)
    typedef std::vector<pixel_type> data_type;

    /// Standard constructor
    BlendBuffer();

    /// Constructor with automatic resize
    BlendBuffer(int _width, int _height);

    /// Write pixel value to position (x,y)
    void put(int _x, int _y, const pixel_type& _pixel);
    
    /// Write pixel value to offset (= x * width + y)
    void put(size_t _offset, const pixel_type& _pixel);

    /// Return pixel on position (x,y)
    pixel_type& operator()(int _x, int _y);
    
    /// Return pixel on position (x,y) (const version)
    pixel_type const& operator()(int _x, int _y) const;
    
    /// Return pixel on offset (= x * width + y)
    pixel_type& operator()(size_t _offset);
    
    /// Return pixel on offset (= x * width + y) (const version)
    pixel_type const& operator()(size_t _offset) const;

    /// Return pixel on position (x,y)
    pixel_type& pixel(int _x, int _y);

    BlendBuffer cropRect(QRect const&) const;

    /// Return pixel on position (x,y) (const version)
    pixel_type const& pixel(int _x, int _y) const;

    /// Return pixel on offset (= x * width + y)
    pixel_type& pixel(size_t _offset);
    
    /// Return pixel on offset (= x * width + y) (const version)
    pixel_type const& pixel(size_t _offset) const;
    
    /// Convert buffer to QImage in 32-bit RGB format
    QImage toQImage() const;
    
    /// Return width of the buffer
    int width() const;
    
    /// Return height of the buffer
    int height() const;

    /// Return const reference to data
    data_type const& data() const;
    
    /// Clear buffer with black color
    void clear();

    /// Clear buffer with specific pixel
    void clear(const pixel_type& _pixel);
    
    /// Returns true if width and height are zero
    bool empty() const;

    /// Returns size of buffer (= width * height)
    size_t size() const;

    /// Resize buffer to given width and height
    void resize(int _width, int _height);
    
  private:
    int width_, height_;
    data_type data_;
  };
}

#endif /* OMNI_BLENDBUFFER_H_ */

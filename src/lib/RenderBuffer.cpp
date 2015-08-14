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

#include <omni/RenderBuffer.h>

#include <QImage>
#include <QColor>
#include <QDebug>

namespace omni
{
  RenderBuffer::RenderBuffer() : width_(0), height_(0) 
  {
  }

  RenderBuffer::RenderBuffer(int _width, int _height)
  {
    resize(_width,_height);
  }

  void RenderBuffer::put(int _x, int _y, const pixel_type& _pixel)
  {
    put(_y*width()+_x,_pixel);
  }
  
  void RenderBuffer::put(size_t _offset, const pixel_type& _pixel)
  {
    data_[_offset] = _pixel;
  }

  RenderBuffer::pixel_type& RenderBuffer::operator()(int _x, int _y)
  {
    return pixel(_x,_y);
  }

  RenderBuffer::pixel_type const& RenderBuffer::operator()(int _x, int _y) const
  {
    return pixel(_x,_y);
  }

  RenderBuffer::pixel_type& RenderBuffer::operator[](size_t _offset)
  {
    return pixel(_offset);
  }

  RenderBuffer::pixel_type const& RenderBuffer::operator[](size_t _offset) const
  {
    return pixel(_offset);
  }

  RenderBuffer::pixel_type& RenderBuffer::pixel(int _x, int _y)
  {
    return pixel(_y*width()+_x);
  }

  RenderBuffer::pixel_type const& RenderBuffer::pixel(int _x, int _y) const
  {
    return pixel(_y*width()+_x);
  }

  RenderBuffer::pixel_type& RenderBuffer::pixel(size_t _offset)
  {
    return data_[_offset];
  }

  RenderBuffer::pixel_type const& RenderBuffer::pixel(size_t _offset) const
  {
    return data_[_offset];
  }
    
  /// Return width of the buffer
  int RenderBuffer::width() const
  {
    return width_;
  }

  /// Return height of the buffer
  int RenderBuffer::height() const
  {
    return height_;
  }


  bool RenderBuffer::empty() const
  {
    return data_.empty();
  }

  size_t RenderBuffer::size() const
  {
    return data_.size();
  }

  void RenderBuffer::clear()
  {
    data_ = data_type(data_.size(),RGBAFloat(0,0,0));
  }
 
  void RenderBuffer::resize(int _width, int _height)
  {
    width_ = _width;
    height_ = _height;
    data_.resize(width_*height_);
  }

  RenderBuffer::data_type const& RenderBuffer::data() const
  {
    return data_;
  }
    
  /// Return pointer to data
  void* RenderBuffer::ptr()
  {
    return static_cast<void*>(data_.data());
  }
}


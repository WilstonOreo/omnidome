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


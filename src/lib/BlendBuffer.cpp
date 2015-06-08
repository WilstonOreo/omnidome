#include <omni/BlendBuffer.h>

#include <QImage>
#include <QColor>

namespace omni
{
  BlendBuffer::BlendBuffer() : width_(0), height_(0) 
  {
  }

  BlendBuffer::BlendBuffer(int _width, int _height)
  {
    resize(_width,_height);
  }

  void BlendBuffer::put(int _x, int _y, const pixel_type& _pixel)
  {
    put(_y*width()+_x,_pixel);
  }
  
  void BlendBuffer::put(size_t _offset, const pixel_type& _pixel)
  {
    data_[_offset] = _pixel;
  }

  BlendBuffer::pixel_type& BlendBuffer::operator()(int _x, int _y)
  {
    return pixel(_x,_y);
  }

  BlendBuffer::pixel_type const& BlendBuffer::operator()(int _x, int _y) const
  {
    return pixel(_x,_y);
  }

  BlendBuffer::pixel_type& BlendBuffer::operator()(size_t _offset)
  {
    return pixel(_offset);
  }

  BlendBuffer::pixel_type const& BlendBuffer::operator()(size_t _offset) const
  {
    return pixel(_offset);
  }


  BlendBuffer::pixel_type& BlendBuffer::pixel(int _x, int _y)
  {
    return pixel(_y*width()+_x);
  }

  BlendBuffer::pixel_type const& BlendBuffer::pixel(int _x, int _y) const
  {
    return pixel(_y*width()+_x);
  }

  BlendBuffer::pixel_type& BlendBuffer::pixel(size_t _offset)
  {
    return data_[_offset];
  }

  BlendBuffer::pixel_type const& BlendBuffer::pixel(size_t _offset) const
  {
    return data_[_offset];
  }
  
  QImage BlendBuffer::toQImage() const
  {
    QImage _output(width(),height(),QImage::Format_RGB32);

    for (int y = 0; y < height(); ++y)
      for (int x = 0; x < width(); ++x)
      {
        auto _p = pixel(x,y);
        _output.setPixel(x,y,QColor(_p,_p,_p).rgb());
      }

    return _output;
  }
    
  /// Return width of the buffer
  int BlendBuffer::width() const
  {
    return width_;
  }

  /// Return height of the buffer
  int BlendBuffer::height() const
  {
    return height_;
  }


  bool BlendBuffer::empty() const
  {
    return data_.empty();
  }

  size_t BlendBuffer::size() const
  {
    return data_.size();
  }

  void BlendBuffer::clear()
  {
    data_ = data_type(data_.size(),0);
  }
  
  void BlendBuffer::clear(const pixel_type& _pixel)
  {
    for (auto& _p: data_)
      _p = _pixel;
  }

  void BlendBuffer::resize(int _width, int _height)
  {
    width_ = _width;
    height_ = _height;
    data_.resize(width_*height_);
  }
}


#ifndef OMNI_RENDERBUFFER_H_
#define OMNI_RENDERBUFFER_H_

#include <cstdint>
#include <cstddef>
#include <vector>

class QImage;

namespace omni
{
  struct RGBAFloat
  {
    RGBAFloat() {}
    RGBAFloat(float r, float g, float b, float a = 1.0) :
      r(r), g(g), b(b), a(a) {} 

    float r,g,b,a;
  };

  /// Render buffer holds an w x h pixel array
  struct RenderBuffer
  {
    /// Our pixel type (32-bit float with R,G,B,A channels)
    typedef RGBAFloat pixel_type;

    /// Our data type (a dynamic array)
    typedef std::vector<pixel_type> data_type;

    /// Standard constructor
    RenderBuffer();
    
    /// Constructor with automatic resize
    RenderBuffer(int _width, int _height);

    /// Write pixel value to position (x,y)
    void put(int _x, int _y, const pixel_type& _pixel);
    
    /// Write pixel value to offset (= x * width + y)
    void put(size_t _offset, const pixel_type& _pixel);

    /// Return pixel on position (x,y)
    pixel_type& operator()(int _x, int _y);
    
    /// Return pixel on position (x,y) (const version)
    pixel_type const& operator()(int _x, int _y) const;
    
    /// Subscript operator for accessing elements by index
    pixel_type& operator[](size_t _offset);
    
    /// Subscript operator for accessing elements by index (const version)
    pixel_type const& operator[](size_t _offset) const;

    /// Return pixel on position (x,y)
    pixel_type& pixel(int _x, int _y);

    /// Return pixel on position (x,y) (const version)
    pixel_type const& pixel(int _x, int _y) const;

    /// Return pixel on offset (= x * width + y)
    pixel_type& pixel(size_t _offset);
    
    /// Return pixel on offset (= x * width + y) (const version)
    pixel_type const& pixel(size_t _offset) const;
     
    /// Return width of the buffer
    int width() const;
    
    /// Return height of the buffer
    int height() const;

    /// Return const reference to data
    data_type const& data() const;
    
    /// Return pointer to data
    void* ptr();

    /// Clear buffer with black color
    void clear();
    
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

#endif /* OMNI_RENDERBUFFER_H_ */

#include <omni/input/EquirectangularTestImage.h>

#include <omni/util.h>

namespace omni
{
  namespace input
  {
    EquirectangularTestImage::EquirectangularTestImage() 
    {
    }

    EquirectangularTestImage::~EquirectangularTestImage()
    {
    }    
      
    QString EquirectangularTestImage::fragmentShaderSource() const
    {  
      static QString _fragmentSrc;
      if (_fragmentSrc.isEmpty())
        _fragmentSrc = util::fileToStr(":/shaders/test_image.frag");
      return _fragmentSrc;
    }
    QString EquirectangularTestImage::vertexShaderSource() const
    {
      static QString _vertSrc;
      if (_vertSrc.isEmpty())
        _vertSrc = util::fileToStr(":/shaders/test_image.vert");
      return _vertSrc;
    }
 
    QSize EquirectangularTestImage::size() const 
    {
      return QSize(4096,2048);
    }
  }
}

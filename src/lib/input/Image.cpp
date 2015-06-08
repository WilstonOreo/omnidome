#include <omni/input/Image.h>

namespace omni
{
  namespace input
  {
    Image::Image()
    {
    }

    Image::~Image()
    {
    }

    void Image::bind()
    {
    }

    void Image::release()
    {
    }

    GLuint Image::textureId() const
    {
      return !texture_ ? 0 : texture_->textureId();
    }

  }
}

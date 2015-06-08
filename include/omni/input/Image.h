#ifndef OMNI_INPUT_IMAGE_H_
#define OMNI_INPUT_IMAGE_H_

#include "Interface.h"

namespace omni
{
  namespace input
  {
    class Image : public Interface
    {
    public:
      OMNI_TYPEID("Image")

      Image();
      ~Image();

      void bind();
      void release();

      GLuint textureId() const;

    private:
      QImage image_;
      std::unique_ptr<QOpenGLTexture> texture_;
      QString path_;
    };
  }
}

#endif /* OMNI_INPUT_IMAGE_H_ */

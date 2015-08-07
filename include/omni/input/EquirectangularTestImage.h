#ifndef OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_
#define OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_

#include "TestImage.h"

namespace omni
{
  namespace input
  {
    class EquirectangularTestImage : 
      public TestImage,
      private Registrar<EquirectangularTestImage>
    {
    public:
      OMNI_TYPEID("EquirectangularTestImage")

      EquirectangularTestImage();
      ~EquirectangularTestImage();

      QSize size() const;

    private:
      QString fragmentShaderSource() const;
      QString vertexShaderSource() const;
    };
  }
}

#endif /* OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_ */

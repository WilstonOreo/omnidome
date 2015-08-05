#ifndef OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_
#define OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject> 
#include "Interface.h"

namespace omni
{
  namespace input
  {
    class EquirectangularTestImage : 
      public Interface,
      private Registrar<EquirectangularTestImage>
    {
    public:
      OMNI_TYPEID("EquirectangularTestImage")

      EquirectangularTestImage();
      ~EquirectangularTestImage();

      GLuint textureId() const;
      void free();
      void update();
      
      QPointF rulerPos() const;
      void setRulerPos(QPointF const&);
      
      QSize size() const;


      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);
 
    private:
      int id_ = 0;
      QPointF rulerPos_;
      bool rulerPosChanged_ = true;
      std::unique_ptr<QOpenGLShaderProgram> shader_;
      std::unique_ptr<QOpenGLFramebufferObject> framebuffer_;
    };
  }
}

#endif /* OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_ */

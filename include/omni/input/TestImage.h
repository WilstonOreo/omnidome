#ifndef OMNI_INPUT_TESTIMAGE_H_
#define OMNI_INPUT_TESTIMAGE_H_

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject> 
#include "Interface.h"

namespace omni
{
  namespace input
  {
    /**@brief Abstract class for test images that are generated with a shader
     **/
    class TestImage : 
      public Interface
    {
    public:
      OMNI_TYPEID("TestImage")

      TestImage();
      virtual ~TestImage();

      GLuint textureId() const;
      void free();
      void update();
      
      QPointF rulerPos() const;
      void setRulerPos(QPointF const&);
     
      virtual void toStream(QDataStream&) const;
      virtual void fromStream(QDataStream&);

    private:
      virtual QString fragmentShaderSource() const = 0;
      virtual QString vertexShaderSource() const = 0;
      virtual void shaderUniformHandler();

      QPointF rulerPos_;
      bool rulerPosChanged_ = true;
      std::unique_ptr<QOpenGLShaderProgram> shader_;
      std::unique_ptr<QOpenGLFramebufferObject> framebuffer_;
    };

  }
}

#endif /* OMNI_INPUT_TESTIMAGE_H_ */

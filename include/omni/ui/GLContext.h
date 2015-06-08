#ifndef OMNI_UI_GLCONTEXT_H
#define OMNI_UI_GLCONTEXT_H

#include <tbd/property.h>
#include <omni/gl.hpp>
#include <QtOpenGL/QGLContext>
#include <memory>

namespace omni
{
  namespace ui
  {
    class GLContext : public QGLContext
    {
    public:
      typedef Texture<Image4ub,GL_TEXTURE_2D> texture_type;
      typedef std::unique_ptr<texture_type> texture_ptr_type;
      typedef std::unique_ptr<Shader> shader_ptr_type;

      GLContext();
      ~GLContext();

      void initialize();/*
      void useTexture(std::string const&, MapMode);
      void useTestImage();
      void useProjImage(int _number);*/
      void free();

      TBD_PROPERTY_REF_RO(shader_ptr_type,shader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,blendShader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,testImageShader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,projImageShader) 
    };
  }
}

#endif /* OMNI_UI_GLCONTEXT_H */

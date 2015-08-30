#ifndef FFGLFBO_H
#define FFGLFBO_H

#include <FFGL.h>
#include <FFGLExtensions.h>

class FFGLFBO
{
public:
  FFGLFBO()
  :m_width(0),
   m_height(0),
   m_glWidth(0),
   m_glHeight(0),
   m_glPixelFormat(0),
   m_glTextureTarget(0),
   m_glTextureHandle(0),
   m_fboHandle(0),
   m_depthBufferHandle(0)
  {}

  int Create(int width, int height, FFGLExtensions &e);
  int BindAsRenderTarget(FFGLExtensions &e);
  int UnbindAsRenderTarget(FFGLExtensions &e);

  FFGLTextureStruct GetTextureInfo();

  void FreeResources(FFGLExtensions &e);
  
  GLuint GetWidth() { return m_width; }
  GLuint GetHeight() { return m_height; }
  GLuint GetFBOHandle() { return m_fboHandle; }

protected:
  GLuint m_width;
  GLuint m_height;
  GLuint m_glWidth;
  GLuint m_glHeight;
  GLuint m_glPixelFormat;
  GLuint m_glTextureTarget;
  GLuint m_glTextureHandle;
  GLuint m_fboHandle;
  GLuint m_depthBufferHandle;
};

#endif

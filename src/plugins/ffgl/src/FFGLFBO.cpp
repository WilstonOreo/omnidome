#include "FFGLFBO.h"

int FFGLFBO::Create(int _width,
                      int _height,
                      FFGLExtensions &e)
{
  int glWidth = 1;
  while (glWidth<_width) glWidth*=2;
  
  int glHeight = 1;
  while (glHeight<_height) glHeight*=2;
  
  m_width = _width;
  m_height = _height;
  m_glWidth = glWidth;
  m_glHeight = glHeight;
  m_glPixelFormat = GL_RGBA8;
  m_glTextureTarget = GL_TEXTURE_2D;

  m_glTextureHandle = 0;
  
  e.glGenFramebuffersEXT(1, &m_fboHandle);

  return 1;
}

int IsTextureResident(GLuint handle)
{
  GLboolean b;

  if (glAreTexturesResident(1, &handle, &b))  
    return 1;

  return 0;
}


int FFGLFBO::BindAsRenderTarget(FFGLExtensions &e)
{
  //make our fbo active
  e.glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboHandle);

  //make sure there's a valid depth buffer attached to it
  if (e.glIsRenderbufferEXT(m_depthBufferHandle)==0)
  {
    e.glGenRenderbuffersEXT(1, &m_depthBufferHandle);
    e.glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferHandle);
    e.glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_glWidth, m_glHeight);
  
    //attach our depth buffer to the fbo
    e.glFramebufferRenderbufferEXT(
      GL_FRAMEBUFFER_EXT,
      GL_DEPTH_ATTACHMENT_EXT,
      GL_RENDERBUFFER_EXT,
      m_depthBufferHandle);
  }

  //make sure we have a valid gl texture attached to it
  if (!IsTextureResident(m_glTextureHandle))
  {
    //get a new one
    glGenTextures(1,&m_glTextureHandle);

    //bind it for some initialization
    glBindTexture(m_glTextureTarget, m_glTextureHandle);

    //this only works if the FBO pixel format
    //is GL_RGBA8. other FBO pixel formats have to
    //define their texture differently
    GLuint pformat = GL_RGBA;
    GLuint ptype = GL_UNSIGNED_BYTE;

    glTexImage2D(
      m_glTextureTarget, //texture target
      0, //mipmap level
      m_glPixelFormat, //gl internal pixel format
      m_glWidth, //gl width
      m_glHeight, //gl height
      0, //no border
      pformat, //pixel format #2
      ptype, //pixel type
      NULL); //null texture image data pointer

    //for now, do not do mipmapping.
    //to do mipmapping, set this to 1, and after we've bound the texture,
    //we would need to call glGenerateMipmapEXT(m_textureTarget) to make sure
    //the mipmaps have been updated
    int do_mipmaps = 0;

    if (do_mipmaps)
      e.glGenerateMipmapEXT(m_glTextureTarget);

    glTexParameteri(m_glTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if (do_mipmaps)
  	  glTexParameteri(m_glTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
      glTexParameteri(m_glTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameteri(m_glTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_glTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //unbind the texture
    glBindTexture(m_glTextureTarget, 0);

    //attach our texture to the FBO
    e.glFramebufferTexture2DEXT(
      GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT0_EXT,
      m_glTextureTarget,
      m_glTextureHandle,
      0);
  }  

  GLenum status = e.glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

  switch(status)
  {
  case GL_FRAMEBUFFER_COMPLETE_EXT:
	//no error
	break;

  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
	return 0;

  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
	//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
	return 0;

  default:
	//FFDebugMessage("Unknown GL_FRAMEBUFFER error");
	return 0;		
  }

  return 1;
}

int FFGLFBO::UnbindAsRenderTarget(FFGLExtensions &e)
{
  e.glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  return 1;
}

FFGLTextureStruct FFGLFBO::GetTextureInfo()
{
  FFGLTextureStruct t;

  t.Width = m_width;
  t.Height = m_height;
  
  t.HardwareWidth = m_glWidth;
  t.HardwareHeight = m_glHeight;
  
  t.Handle = m_glTextureHandle;

  return t;
}


void FFGLFBO::FreeResources(FFGLExtensions &e)
{
  if (m_fboHandle)
  {
    e.glDeleteFramebuffersEXT(1, &m_fboHandle);
    m_fboHandle = 0;
  }

  if (m_depthBufferHandle)
  {
    e.glDeleteRenderBuffersEXT(1, &m_depthBufferHandle);
    m_depthBufferHandle = 0;
  }

  if (m_glTextureHandle)
  {
    glDeleteTextures(1, &m_glTextureHandle);
    m_glTextureHandle = 0;
  }
}

#include <FFGL.h>
#include <FFGLExtensions.h>
#include <stdlib.h>
#include <string.h>

#ifdef TARGET_OS_MAC
//for the NS* calls in GetProcAddress
#include <mach-o/dyld.h>
#endif
 
#ifdef __linux__
#include <GL/glx.h>
#endif

FFGLExtensions::FFGLExtensions()
{
  memset(this, 0, sizeof(FFGLExtensions));
}

void FFGLExtensions::Initialize()
{
#ifdef _WIN32
  InitWGLEXTSwapControl();
#endif

  InitMultitexture();
  InitARBShaderObjects();
  InitEXTFramebufferObject();
}

void *FFGLExtensions::GetProcAddress(char *name)
{

#ifdef _WIN32

  void *result = wglGetProcAddress(name);

  if (result!=NULL)
    return result;
    
#else

#ifdef TARGET_OS_MAC
  
  // Prepend a '_' for the Unix C symbol mangling convention
  int symbolLength = strlen(name) + 2; //1 for the _, another for the trailing null
  char symbolName[1024];
  if (symbolLength>sizeof(symbolName))
  {
    //symbol name too long;
    throw;
    return NULL;
  }
  
  strcpy(symbolName + 1, name);
  symbolName[0] = '_';

  NSSymbol symbol = NULL;
  
  if (NSIsSymbolNameDefined(symbolName))
    symbol = NSLookupAndBindSymbol(symbolName);
  
  if (symbol!=NULL)
  {
    return NSAddressOfSymbol(symbol);
  }

#else

#ifdef __linux__

  void *result = (void *)(unsigned)glXGetProcAddress((const GLubyte *)name);

  if (result!=NULL)
    return result;

#else
  
#error Define this for your OS
  
#endif
#endif
#endif
  throw;//this will be caught by one of the Init() functions below
  return NULL;
}

void FFGLExtensions::InitMultitexture()
{
  try
  {  
  glActiveTexture = (glActiveTexturePROC)(unsigned)GetProcAddress("glActiveTexture");
  glClientActiveTexture = (glClientActiveTexturePROC)(unsigned)GetProcAddress("glClientActiveTexture");

  glMultiTexCoord1d = (glMultiTexCoord1dPROC)(unsigned)GetProcAddress("glMultiTexCoord1d");
  glMultiTexCoord1dv = (glMultiTexCoord1dvPROC)(unsigned)GetProcAddress("glMultiTexCoord1dv");
  glMultiTexCoord1f = (glMultiTexCoord1fPROC)(unsigned)GetProcAddress("glMultiTexCoord1f");
  glMultiTexCoord1fv = (glMultiTexCoord1fvPROC)(unsigned)GetProcAddress("glMultiTexCoord1fv");
  glMultiTexCoord1i = (glMultiTexCoord1iPROC)(unsigned)GetProcAddress("glMultiTexCoord1i");
  glMultiTexCoord1iv = (glMultiTexCoord1ivPROC)(unsigned)GetProcAddress("glMultiTexCoord1iv");
  glMultiTexCoord1s = (glMultiTexCoord1sPROC)(unsigned)GetProcAddress("glMultiTexCoord1s");
  glMultiTexCoord1sv = (glMultiTexCoord1svPROC)(unsigned)GetProcAddress("glMultiTexCoord1sv");

  glMultiTexCoord2d = (glMultiTexCoord2dPROC)(unsigned)GetProcAddress("glMultiTexCoord2d");
  glMultiTexCoord2dv = (glMultiTexCoord2dvPROC)(unsigned)GetProcAddress("glMultiTexCoord2dv");
  glMultiTexCoord2f = (glMultiTexCoord2fPROC)(unsigned)GetProcAddress("glMultiTexCoord2f");
  glMultiTexCoord2fv = (glMultiTexCoord2fvPROC)(unsigned)GetProcAddress("glMultiTexCoord2fv");
  glMultiTexCoord2i = (glMultiTexCoord2iPROC)(unsigned)GetProcAddress("glMultiTexCoord2i");
  glMultiTexCoord2iv = (glMultiTexCoord2ivPROC)(unsigned)GetProcAddress("glMultiTexCoord2iv");
  glMultiTexCoord2s = (glMultiTexCoord2sPROC)(unsigned)GetProcAddress("glMultiTexCoord2s");
  glMultiTexCoord2sv = (glMultiTexCoord2svPROC)(unsigned)GetProcAddress("glMultiTexCoord2sv");

  glMultiTexCoord3d = (glMultiTexCoord3dPROC)(unsigned)GetProcAddress("glMultiTexCoord3d");
  glMultiTexCoord3dv = (glMultiTexCoord3dvPROC)(unsigned)GetProcAddress("glMultiTexCoord3dv");
  glMultiTexCoord3f = (glMultiTexCoord3fPROC)(unsigned)GetProcAddress("glMultiTexCoord3f");
  glMultiTexCoord3fv = (glMultiTexCoord3fvPROC)(unsigned)GetProcAddress("glMultiTexCoord3fv");
  glMultiTexCoord3i = (glMultiTexCoord3iPROC)(unsigned)GetProcAddress("glMultiTexCoord3i");
  glMultiTexCoord3iv = (glMultiTexCoord3ivPROC)(unsigned)GetProcAddress("glMultiTexCoord3iv");
  glMultiTexCoord3s = (glMultiTexCoord3sPROC)(unsigned)GetProcAddress("glMultiTexCoord3s");
  glMultiTexCoord3sv = (glMultiTexCoord3svPROC)(unsigned)GetProcAddress("glMultiTexCoord3sv");

  glMultiTexCoord4d = (glMultiTexCoord4dPROC)(unsigned)GetProcAddress("glMultiTexCoord4d");
  glMultiTexCoord4dv = (glMultiTexCoord4dvPROC)(unsigned)GetProcAddress("glMultiTexCoord4dv");
  glMultiTexCoord4f = (glMultiTexCoord4fPROC)(unsigned)GetProcAddress("glMultiTexCoord4f");
  glMultiTexCoord4fv = (glMultiTexCoord4fvPROC)(unsigned)GetProcAddress("glMultiTexCoord4fv");
  glMultiTexCoord4i = (glMultiTexCoord4iPROC)(unsigned)GetProcAddress("glMultiTexCoord4i");
  glMultiTexCoord4iv = (glMultiTexCoord4ivPROC)(unsigned)GetProcAddress("glMultiTexCoord4iv");
  glMultiTexCoord4s = (glMultiTexCoord4sPROC)(unsigned)GetProcAddress("glMultiTexCoord4s");
  glMultiTexCoord4sv = (glMultiTexCoord4svPROC)(unsigned)GetProcAddress("glMultiTexCoord4sv");
  }
  catch (...)
  {
    //not supported
    multitexture = 0;
    return;
  }
  
  //if we get this far w/no exceptions, ARB_shader_objects shoudl be fully
  //supported
  multitexture = 1;
}

void FFGLExtensions::InitARBShaderObjects()
{
  try
  {

  glDeleteObjectARB = (glDeleteObjectARBPROC)(unsigned)GetProcAddress("glDeleteObjectARB");
  glGetHandleARB = (glGetHandleARBPROC)(unsigned)GetProcAddress("glGetHandleARB");
  glDetachObjectARB = (glDetachObjectARBPROC)(unsigned)GetProcAddress("glDetachObjectARB");
  glCreateShaderObjectARB = (glCreateShaderObjectARBPROC)(unsigned)GetProcAddress("glCreateShaderObjectARB");
  glShaderSourceARB = (glShaderSourceARBPROC)(unsigned)GetProcAddress("glShaderSourceARB");
  glCompileShaderARB = (glCompileShaderARBPROC)(unsigned)GetProcAddress("glCompileShaderARB");
  glCreateProgramObjectARB = (glCreateProgramObjectARBPROC)(unsigned)GetProcAddress("glCreateProgramObjectARB");
  glAttachObjectARB = (glAttachObjectARBPROC)(unsigned)GetProcAddress("glAttachObjectARB");
  glLinkProgramARB = (glLinkProgramARBPROC)(unsigned)GetProcAddress("glLinkProgramARB");
  glUseProgramObjectARB = (glUseProgramObjectARBPROC)(unsigned)GetProcAddress("glUseProgramObjectARB");
  glValidateProgramARB = (glValidateProgramARBPROC)(unsigned)GetProcAddress("glValidateProgramARB");
  glUniform1fARB = (glUniform1fARBPROC)(unsigned)GetProcAddress("glUniform1fARB");
  glUniform2fARB = (glUniform2fARBPROC)(unsigned)GetProcAddress("glUniform2fARB");
  glUniform3fARB = (glUniform3fARBPROC)(unsigned)GetProcAddress("glUniform3fARB");
  glUniform4fARB = (glUniform4fARBPROC)(unsigned)GetProcAddress("glUniform4fARB");
  glUniform1iARB = (glUniform1iARBPROC)(unsigned)GetProcAddress("glUniform1iARB");
  glUniform2iARB = (glUniform2iARBPROC)(unsigned)GetProcAddress("glUniform2iARB");
  glUniform3iARB = (glUniform3iARBPROC)(unsigned)GetProcAddress("glUniform3iARB");
  glUniform4iARB = (glUniform4iARBPROC)(unsigned)GetProcAddress("glUniform4iARB");
  glUniform1fvARB = (glUniform1fvARBPROC)(unsigned)GetProcAddress("glUniform1fvARB");
  glUniform2fvARB = (glUniform2fvARBPROC)(unsigned)GetProcAddress("glUniform2fvARB");
  glUniform3fvARB = (glUniform3fvARBPROC)(unsigned)GetProcAddress("glUniform3fvARB");
  glUniform4fvARB = (glUniform4fvARBPROC)(unsigned)GetProcAddress("glUniform4fvARB");
  glUniform1ivARB = (glUniform1ivARBPROC)(unsigned)GetProcAddress("glUniform1ivARB");
  glUniform2ivARB = (glUniform2ivARBPROC)(unsigned)GetProcAddress("glUniform2ivARB");
  glUniform3ivARB = (glUniform3ivARBPROC)(unsigned)GetProcAddress("glUniform3ivARB");
  glUniform4ivARB = (glUniform4ivARBPROC)(unsigned)GetProcAddress("glUniform4ivARB");
  glUniformMatrix2fvARB = (glUniformMatrix2fvARBPROC)(unsigned)GetProcAddress("glUniformMatrix2fvARB");
  glUniformMatrix3fvARB = (glUniformMatrix3fvARBPROC)(unsigned)GetProcAddress("glUniformMatrix3fvARB");
  glUniformMatrix4fvARB = (glUniformMatrix4fvARBPROC)(unsigned)GetProcAddress("glUniformMatrix4fvARB");
  glGetObjectParameterfvARB = (glGetObjectParameterfvARBPROC)(unsigned)GetProcAddress("glGetObjectParameterfvARB");
  glGetObjectParameterivARB = (glGetObjectParameterivARBPROC)(unsigned)GetProcAddress("glGetObjectParameterivARB");
  glGetInfoLogARB = (glGetInfoLogARBPROC)(unsigned)GetProcAddress("glGetInfoLogARB");
  glGetAttachedObjectsARB = (glGetAttachedObjectsARBPROC)(unsigned)GetProcAddress("glGetAttachedObjectsARB");
  glGetUniformLocationARB = (glGetUniformLocationARBPROC)(unsigned)GetProcAddress("glGetUniformLocationARB");
  glGetActiveUniformARB = (glGetActiveUniformARBPROC)(unsigned)GetProcAddress("glGetActiveUniformARB");
  glGetUniformfvARB = (glGetUniformfvARBPROC)(unsigned)GetProcAddress("glGetUniformfvARB");
  glGetUniformivARB = (glGetUniformivARBPROC)(unsigned)GetProcAddress("glGetUniformivARB");
  glGetShaderSourceARB = (glGetShaderSourceARBPROC)(unsigned)GetProcAddress("glGetShaderSourceARB");

  }
  catch (...)
  {
    //not supported
    ARB_shader_objects = 0;
    return;
  }

  //if we get this far w/no exceptions, ARB_shader_objects shoudl be fully
  //supported
  ARB_shader_objects = 1;
}

void FFGLExtensions::InitEXTFramebufferObject()
{
  try
  {

  glBindFramebufferEXT = (glBindFramebufferEXTPROC)(unsigned)GetProcAddress("glBindFramebufferEXT");
  glBindRenderbufferEXT = (glBindRenderbufferEXTPROC)(unsigned)GetProcAddress("glBindRenderbufferEXT");
  glCheckFramebufferStatusEXT = (glCheckFramebufferStatusEXTPROC)(unsigned)GetProcAddress("glCheckFramebufferStatusEXT");
  glDeleteFramebuffersEXT = (glDeleteFramebuffersEXTPROC)(unsigned)GetProcAddress("glDeleteFramebuffersEXT");
  glDeleteRenderBuffersEXT = (glDeleteRenderBuffersEXTPROC)(unsigned)GetProcAddress("glDeleteRenderbuffersEXT");
  glFramebufferRenderbufferEXT = (glFramebufferRenderbufferEXTPROC)(unsigned)GetProcAddress("glFramebufferRenderbufferEXT");
  glFramebufferTexture1DEXT = (glFramebufferTexture1DEXTPROC)(unsigned)GetProcAddress("glFramebufferTexture1DEXT");
  glFramebufferTexture2DEXT = (glFramebufferTexture2DEXTPROC)(unsigned)GetProcAddress("glFramebufferTexture2DEXT");
  glFramebufferTexture3DEXT = (glFramebufferTexture3DEXTPROC)(unsigned)GetProcAddress("glFramebufferTexture3DEXT");
  glGenFramebuffersEXT = (glGenFramebuffersEXTPROC)(unsigned)GetProcAddress("glGenFramebuffersEXT");
  glGenRenderbuffersEXT = (glGenRenderbuffersEXTPROC)(unsigned)GetProcAddress("glGenRenderbuffersEXT");
  glGenerateMipmapEXT = (glGenerateMipmapEXTPROC)(unsigned)GetProcAddress("glGenerateMipmapEXT");
  glGetFramebufferAttachmentParameterivEXT = (glGetFramebufferAttachmentParameterivEXTPROC)(unsigned)GetProcAddress("glGetFramebufferAttachmentParameterivEXT");
  glGetRenderbufferParameterivEXT = (glGetRenderbufferParameterivEXTPROC)(unsigned)GetProcAddress("glGetRenderbufferParameterivEXT");
  glIsFramebufferEXT = (glIsFramebufferEXTPROC)(unsigned)GetProcAddress("glIsFramebufferEXT");
  glIsRenderbufferEXT = (glIsRenderbufferEXTPROC)(unsigned)GetProcAddress("glIsRenderbufferEXT");
  glRenderbufferStorageEXT = (glRenderbufferStorageEXTPROC)(unsigned)GetProcAddress("glRenderbufferStorageEXT");

  }
  catch (...)
  {
    //not supported
    EXT_framebuffer_object = 0;
    return;
  }

  EXT_framebuffer_object = 1;
}

#ifdef _WIN32
void FFGLExtensions::InitWGLEXTSwapControl()
{
  try
  {
  wglSwapIntervalEXT = (wglSwapIntervalEXTPROC) GetProcAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (wglGetSwapIntervalEXTPROC) GetProcAddress("wglGetSwapIntervalEXT");
  }
  catch (...)
  {
    //not supported
    WGL_EXT_swap_control = 0;
    return;
  }

  WGL_EXT_swap_control = 1;
}
#endif

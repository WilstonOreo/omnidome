#include "FFGLShader.h"

#include <string>
#include <iostream>

FFGLShader::FFGLShader()
{
  m_linkStatus = 0;
  m_glProgram = 0;
  m_glVertexShader = 0;
  m_glFragmentShader = 0;
  m_extensions = NULL;
}

void FFGLShader::CreateGLResources()
{
  if (m_extensions==NULL)
    return;

  if (m_glProgram==0)
    m_glProgram = m_extensions->glCreateProgramObjectARB();

  if (m_glVertexShader==0)
    m_glVertexShader = m_extensions->glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

  if (m_glFragmentShader==0)
    m_glFragmentShader = m_extensions->glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
}

void FFGLShader::FreeGLResources()
{
  if (m_extensions==NULL)
    return;

  if (m_glFragmentShader)
  {
    m_extensions->glDeleteObjectARB(m_glFragmentShader);
    m_glFragmentShader = 0;
  }

  if (m_glVertexShader)
  {
    m_extensions->glDeleteObjectARB(m_glVertexShader);
    m_glVertexShader = 0;
  }

  if (m_glProgram)
  {
    m_extensions->glDeleteObjectARB(m_glProgram);
    m_glProgram = 0;
  }
}

int FFGLShader::BindShader()
{
  if (m_extensions==NULL)
    return 0;

  //make sure the program type is supported
  if (m_extensions->ARB_shader_objects==0)
    return 0;

  if (m_glProgram==0)
    return 0;

  m_extensions->glUseProgramObjectARB(m_glProgram);

  return 1;
}

int FFGLShader::UnbindShader()
{
  if (m_extensions->ARB_shader_objects==0)
    return 0;

  m_extensions->glUseProgramObjectARB(0);

  return 1;
}

FFGLShader::~FFGLShader()
{
}

void FFGLShader::SetExtensions(FFGLExtensions *e)
{
  m_extensions = e;
}

int FFGLShader::Compile(const char *vtxProgram, const char *fragProgram)
{
  if (m_extensions==NULL)
    return 0;
  
  if (m_glProgram==0)
    CreateGLResources();
  
  int doLink = 0;

  //if we can compile a fragment shader, do it.
  if (m_glFragmentShader!=0 &&
      m_glProgram!=0 &&
      fragProgram!=NULL && fragProgram[0]!=0)
  {
    const char *strings[] =
    {
      fragProgram,
      NULL
    };

    // Load Shader Sources
    m_extensions->glShaderSourceARB(m_glFragmentShader, 1, strings, NULL);

    // Compile The Shaders
    m_extensions->glCompileShaderARB(m_glFragmentShader);
    
    GLint compileSuccess;
    
    m_extensions->glGetObjectParameterivARB(
      m_glFragmentShader,
      GL_OBJECT_COMPILE_STATUS_ARB,
      &compileSuccess);

    if (compileSuccess == GL_TRUE)
    {
      //attach it to the program
      m_extensions->glAttachObjectARB(m_glProgram, m_glFragmentShader);
      doLink = 1;
    }
    else
    {
      //get the log so we can peek at the error string
      char log[1024];
      GLsizei returnedLength;
      
      m_extensions->glGetInfoLogARB(
        m_glFragmentShader,
        sizeof(log)-1,
        &returnedLength,
        log);

      log[returnedLength] = 0;
      std::cout << std::string(log);
      int a;
      a=0;
    }
  }

  //if we can compile a vertex shader, do it
  if (m_glVertexShader!=0 &&
      m_glProgram!=0 &&
      vtxProgram!=0 && vtxProgram[0]!=0)
  {
    const char *strings[] =
    {
      vtxProgram,
      NULL
    };

    // Load Shader Sources
    m_extensions->glShaderSourceARB(m_glVertexShader, 1, strings, NULL);

    // Compile The Shaders
    m_extensions->glCompileShaderARB(m_glVertexShader);

    GLint compileSuccess;
      
    m_extensions->glGetObjectParameterivARB(
      m_glVertexShader,
      GL_OBJECT_COMPILE_STATUS_ARB,
      &compileSuccess);

    if (compileSuccess == GL_TRUE)
    {
      //attach it to the program
      m_extensions->glAttachObjectARB(m_glProgram, m_glVertexShader);
      doLink = 1;
    } 
    else
    {
      //get the log so we can peek at the error string
      char log[1024];
      GLsizei returnedLength;
      
      m_extensions->glGetInfoLogARB(
        m_glVertexShader,
        sizeof(log)-1,
        &returnedLength,
        log);

      log[returnedLength] = 0;
      std::cout << std::string(log);
      int a;
      a=0;
    }     
  }
  
  //check if linking worked
  GLint linkSuccess = 0;

  if (doLink)
  {
    // Link The Program Object
    m_extensions->glLinkProgramARB(m_glProgram);
      
	  m_extensions->glGetObjectParameterivARB(
	    m_glProgram,
	    GL_OBJECT_LINK_STATUS_ARB,
	    &linkSuccess);  
  }
  
  m_linkStatus = linkSuccess;
  
  return linkSuccess;
}

GLuint FFGLShader::FindUniform(const char *name)
{
  return m_extensions->glGetUniformLocationARB(m_glProgram,name);
}

#ifndef FFGLEXTENSIONS_H
#define FFGLEXTENSIONS_H

//originally this file came from SGI.
//Lev Povalahev extended it with his great library "GLew".

//The GLew headers have been reduced to a minimal set of extensions
//required by the sample host and plugins in the FFGL SDK by
//Trey Harrison - www.harrisondigitalmedia.com

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
** 
** http://oss.sgi.com/projects/FreeB
** 
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
** 
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
** 
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
*/

/*  Most parts copyright (c) 2001-2002 Lev Povalahev under this lisence: */

/* ----------------------------------------------------------------------------
Copyright (c) 2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.
    * The name of the author may be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#ifdef _WIN32

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#else

#ifdef TARGET_OS_MAC

#define APIENTRY

#else
#ifdef __linux__

// APIENTRY is defined in gl.h

#else

#error #define APIENTRY for your platform

#endif
#endif
#endif

///////////////
// OpenGL 1.2
///////////////

#define GL_CLAMP_TO_EDGE                                    0x812F
#define GL_TEXTURE_3D                                       0x806F

///////////////
// OpenGL 1.3 Multitexture
///////////////

#define GL_TEXTURE0                                             0x84C0
#define GL_TEXTURE1                                             0x84C1
#define GL_TEXTURE2                                             0x84C2
#define GL_TEXTURE3                                             0x84C3
#define GL_TEXTURE4                                             0x84C4
#define GL_TEXTURE5                                             0x84C5
#define GL_TEXTURE6                                             0x84C6
#define GL_TEXTURE7                                             0x84C7
#define GL_TEXTURE8                                             0x84C8
#define GL_TEXTURE9                                             0x84C9
#define GL_TEXTURE10                                            0x84CA
#define GL_TEXTURE11                                            0x84CB
#define GL_TEXTURE12                                            0x84CC
#define GL_TEXTURE13                                            0x84CD
#define GL_TEXTURE14                                            0x84CE
#define GL_TEXTURE15                                            0x84CF
#define GL_TEXTURE16                                            0x84D0
#define GL_TEXTURE17                                            0x84D1
#define GL_TEXTURE18                                            0x84D2
#define GL_TEXTURE19                                            0x84D3
#define GL_TEXTURE20                                            0x84D4
#define GL_TEXTURE21                                            0x84D5
#define GL_TEXTURE22                                            0x84D6
#define GL_TEXTURE23                                            0x84D7
#define GL_TEXTURE24                                            0x84D8
#define GL_TEXTURE25                                            0x84D9
#define GL_TEXTURE26                                            0x84DA
#define GL_TEXTURE27                                            0x84DB
#define GL_TEXTURE28                                            0x84DC
#define GL_TEXTURE29                                            0x84DD
#define GL_TEXTURE30                                            0x84DE
#define GL_TEXTURE31                                            0x84DF
#define GL_ACTIVE_TEXTURE                                       0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE                                0x84E1
#define GL_MAX_TEXTURE_UNITS                                    0x84E2

typedef void (APIENTRY *glActiveTexturePROC) (GLenum texture );
typedef void (APIENTRY *glClientActiveTexturePROC) (GLenum texture );
typedef void (APIENTRY *glMultiTexCoord1dPROC) (GLenum target, GLdouble s );
typedef void (APIENTRY *glMultiTexCoord1dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY *glMultiTexCoord1fPROC) (GLenum target, GLfloat s );
typedef void (APIENTRY *glMultiTexCoord1fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY *glMultiTexCoord1iPROC) (GLenum target, GLint s );
typedef void (APIENTRY *glMultiTexCoord1ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY *glMultiTexCoord1sPROC) (GLenum target, GLshort s );
typedef void (APIENTRY *glMultiTexCoord1svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY *glMultiTexCoord2dPROC) (GLenum target, GLdouble s, GLdouble t );
typedef void (APIENTRY *glMultiTexCoord2dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY *glMultiTexCoord2fPROC) (GLenum target, GLfloat s, GLfloat t );
typedef void (APIENTRY *glMultiTexCoord2fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY *glMultiTexCoord2iPROC) (GLenum target, GLint s, GLint t );
typedef void (APIENTRY *glMultiTexCoord2ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY *glMultiTexCoord2sPROC) (GLenum target, GLshort s, GLshort t );
typedef void (APIENTRY *glMultiTexCoord2svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY *glMultiTexCoord3dPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r );
typedef void (APIENTRY *glMultiTexCoord3dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY *glMultiTexCoord3fPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r );
typedef void (APIENTRY *glMultiTexCoord3fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY *glMultiTexCoord3iPROC) (GLenum target, GLint s, GLint t, GLint r );
typedef void (APIENTRY *glMultiTexCoord3ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY *glMultiTexCoord3sPROC) (GLenum target, GLshort s, GLshort t, GLshort r );
typedef void (APIENTRY *glMultiTexCoord3svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY *glMultiTexCoord4dPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
typedef void (APIENTRY *glMultiTexCoord4dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY *glMultiTexCoord4fPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
typedef void (APIENTRY *glMultiTexCoord4fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY *glMultiTexCoord4iPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q );
typedef void (APIENTRY *glMultiTexCoord4ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY *glMultiTexCoord4sPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
typedef void (APIENTRY *glMultiTexCoord4svPROC) (GLenum target, const GLshort *v );

//////////////
//OpenGL 1.4
///////////////

#define GL_GENERATE_MIPMAP                                  0x8191
#define GL_GENERATE_MIPMAP_HINT                             0x8192
#define GL_DEPTH_COMPONENT16                                0x81A5
#define GL_DEPTH_COMPONENT24                                0x81A6
#define GL_DEPTH_COMPONENT32                                0x81A7

//////////////////
//EXT_framebuffer_object
//////////////////

#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT                0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT                        0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT                          0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT                         0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT           0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT           0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT         0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT    0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT                         0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT            0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT    0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT  0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT            0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT               0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT           0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT           0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT                      0x8CDD
#define GL_FRAMEBUFFER_STATUS_ERROR_EXT                     0x8CDE
#define GL_MAX_COLOR_ATTACHMENTS_EXT                        0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT                            0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT                            0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT                            0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT                            0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT                            0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT                            0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT                            0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT                            0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT                            0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT                            0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT                           0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT                           0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT                           0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT                           0x8CED
#define GL_COLOR_ATTACHMENT14_EXT                           0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT                           0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT                             0x8D00
#define GL_STENCIL_ATTACHMENT_EXT                           0x8D20
#define GL_FRAMEBUFFER_EXT                                  0x8D40
#define GL_RENDERBUFFER_EXT                                 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT                           0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT                          0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT                 0x8D44
#define GL_STENCIL_INDEX_EXT                                0x8D45
#define GL_STENCIL_INDEX1_EXT                               0x8D46
#define GL_STENCIL_INDEX4_EXT                               0x8D47
#define GL_STENCIL_INDEX8_EXT                               0x8D48
#define GL_STENCIL_INDEX16_EXT                              0x8D49

typedef void   (APIENTRY *glBindFramebufferEXTPROC) (GLenum target, GLuint framebuffer);
typedef void   (APIENTRY *glBindRenderbufferEXTPROC) (GLenum target, GLuint renderbuffer);
typedef GLenum (APIENTRY *glCheckFramebufferStatusEXTPROC) (GLenum target);
typedef void   (APIENTRY *glDeleteFramebuffersEXTPROC) (GLsizei n, const GLuint* framebuffers);
typedef void   (APIENTRY *glDeleteRenderBuffersEXTPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void   (APIENTRY *glFramebufferRenderbufferEXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void   (APIENTRY *glFramebufferTexture1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void   (APIENTRY *glFramebufferTexture2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void   (APIENTRY *glFramebufferTexture3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void   (APIENTRY *glGenFramebuffersEXTPROC) (GLsizei n, GLuint* framebuffers);
typedef void   (APIENTRY *glGenRenderbuffersEXTPROC) (GLsizei n, GLuint* renderbuffers);
typedef void   (APIENTRY *glGenerateMipmapEXTPROC) (GLenum target);
typedef void   (APIENTRY *glGetFramebufferAttachmentParameterivEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void   (APIENTRY *glGetRenderbufferParameterivEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (APIENTRY *glIsFramebufferEXTPROC) (GLuint framebuffer);
typedef GLboolean (APIENTRY *glIsRenderbufferEXTPROC) (GLuint renderbuffer);
typedef void (APIENTRY *glRenderbufferStorageEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);


//////////////////////
//EXT_TEXTURE_RECTANGLE
//(#'s are the same as NV_TEXTURE_RECTANGLE and ARB_TEXTURE_RECTANGLE)
///////////////////////
#define GL_TEXTURE_RECTANGLE_EXT          0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT  0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT    0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT 0x84F8


///////////////////////
// GL_ARB_shader_objects
///////////////////////
#define GL_FRAGMENT_SHADER_ARB            0x8B30
#define GL_VERTEX_SHADER_ARB              0x8B31
#define GL_PROGRAM_OBJECT_ARB             0x8B40
#define GL_SHADER_OBJECT_ARB              0x8B48
#define GL_OBJECT_TYPE_ARB                0x8B4E
#define GL_OBJECT_SUBTYPE_ARB             0x8B4F
#define GL_FLOAT_VEC2_ARB                 0x8B50
#define GL_FLOAT_VEC3_ARB                 0x8B51
#define GL_FLOAT_VEC4_ARB                 0x8B52
#define GL_INT_VEC2_ARB                   0x8B53
#define GL_INT_VEC3_ARB                   0x8B54
#define GL_INT_VEC4_ARB                   0x8B55
#define GL_BOOL_ARB                       0x8B56
#define GL_BOOL_VEC2_ARB                  0x8B57
#define GL_BOOL_VEC3_ARB                  0x8B58
#define GL_BOOL_VEC4_ARB                  0x8B59
#define GL_FLOAT_MAT2_ARB                 0x8B5A
#define GL_FLOAT_MAT3_ARB                 0x8B5B
#define GL_FLOAT_MAT4_ARB                 0x8B5C
#define GL_OBJECT_DELETE_STATUS_ARB       0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB      0x8B81
#define GL_OBJECT_LINK_STATUS_ARB         0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB     0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB     0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB    0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB     0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88

/* GL types for handling shader object handles and characters */
typedef char GLcharARB;		/* native character */
typedef unsigned int GLhandleARB;	/* shader object handle */

typedef void (APIENTRY * glDeleteObjectARBPROC) (GLhandleARB);
typedef GLhandleARB (APIENTRY * glGetHandleARBPROC) (GLenum);
typedef void (APIENTRY * glDetachObjectARBPROC) (GLhandleARB, GLhandleARB);
typedef GLhandleARB (APIENTRY * glCreateShaderObjectARBPROC) (GLenum);
typedef void (APIENTRY * glShaderSourceARBPROC) (GLhandleARB, GLsizei, const GLcharARB* *, const GLint *);
typedef void (APIENTRY * glCompileShaderARBPROC) (GLhandleARB);
typedef GLhandleARB (APIENTRY * glCreateProgramObjectARBPROC) (void);
typedef void (APIENTRY * glAttachObjectARBPROC) (GLhandleARB, GLhandleARB);
typedef void (APIENTRY * glLinkProgramARBPROC) (GLhandleARB);
typedef void (APIENTRY * glUseProgramObjectARBPROC) (GLhandleARB);
typedef void (APIENTRY * glValidateProgramARBPROC) (GLhandleARB);
typedef void (APIENTRY * glUniform1fARBPROC) (GLint, GLfloat);
typedef void (APIENTRY * glUniform2fARBPROC) (GLint, GLfloat, GLfloat);
typedef void (APIENTRY * glUniform3fARBPROC) (GLint, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY * glUniform4fARBPROC) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY * glUniform1iARBPROC) (GLint, GLint);
typedef void (APIENTRY * glUniform2iARBPROC) (GLint, GLint, GLint);
typedef void (APIENTRY * glUniform3iARBPROC) (GLint, GLint, GLint, GLint);
typedef void (APIENTRY * glUniform4iARBPROC) (GLint, GLint, GLint, GLint, GLint);
typedef void (APIENTRY * glUniform1fvARBPROC) (GLint, GLsizei, const GLfloat *);
typedef void (APIENTRY * glUniform2fvARBPROC) (GLint, GLsizei, const GLfloat *);
typedef void (APIENTRY * glUniform3fvARBPROC) (GLint, GLsizei, const GLfloat *);
typedef void (APIENTRY * glUniform4fvARBPROC) (GLint, GLsizei, const GLfloat *);
typedef void (APIENTRY * glUniform1ivARBPROC) (GLint, GLsizei, const GLint *);
typedef void (APIENTRY * glUniform2ivARBPROC) (GLint, GLsizei, const GLint *);
typedef void (APIENTRY * glUniform3ivARBPROC) (GLint, GLsizei, const GLint *);
typedef void (APIENTRY * glUniform4ivARBPROC) (GLint, GLsizei, const GLint *);
typedef void (APIENTRY * glUniformMatrix2fvARBPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (APIENTRY * glUniformMatrix3fvARBPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (APIENTRY * glUniformMatrix4fvARBPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (APIENTRY * glGetObjectParameterfvARBPROC) (GLhandleARB, GLenum, GLfloat *);
typedef void (APIENTRY * glGetObjectParameterivARBPROC) (GLhandleARB, GLenum, GLint *);
typedef void (APIENTRY * glGetInfoLogARBPROC) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
typedef void (APIENTRY * glGetAttachedObjectsARBPROC) (GLhandleARB, GLsizei, GLsizei *, GLhandleARB *);
typedef GLint (APIENTRY * glGetUniformLocationARBPROC) (GLhandleARB, const GLcharARB *);
typedef void (APIENTRY * glGetActiveUniformARBPROC) (GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
typedef void (APIENTRY * glGetUniformfvARBPROC) (GLhandleARB, GLint, GLfloat *);
typedef void (APIENTRY * glGetUniformivARBPROC) (GLhandleARB, GLint, GLint *);
typedef void (APIENTRY * glGetShaderSourceARBPROC) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);

#ifdef _WIN32

//////////////////
//WGL_EXT_SWAP_CONTROL
//////////////////
typedef BOOL (APIENTRY *wglSwapIntervalEXTPROC) (int interval);
typedef int (APIENTRY *wglGetSwapIntervalEXTPROC) (void);

#endif

/////////////////////////////////////////////////////
///////////////
//classes to handle extensions
///////////////
/////////////////////////////////////////////////////

class FFGLExtensions
{
public:
  FFGLExtensions();
  
  void Initialize();
  
  //Multitexture
  int multitexture;
  glActiveTexturePROC glActiveTexture;
  glClientActiveTexturePROC glClientActiveTexture;
  glMultiTexCoord1dPROC glMultiTexCoord1d;
  glMultiTexCoord1dvPROC glMultiTexCoord1dv;
  glMultiTexCoord1fPROC glMultiTexCoord1f;
  glMultiTexCoord1fvPROC glMultiTexCoord1fv;
  glMultiTexCoord1iPROC glMultiTexCoord1i;
  glMultiTexCoord1ivPROC glMultiTexCoord1iv;
  glMultiTexCoord1sPROC glMultiTexCoord1s;
  glMultiTexCoord1svPROC glMultiTexCoord1sv;
  glMultiTexCoord2dPROC glMultiTexCoord2d;
  glMultiTexCoord2dvPROC glMultiTexCoord2dv;
  glMultiTexCoord2fPROC glMultiTexCoord2f;
  glMultiTexCoord2fvPROC glMultiTexCoord2fv;
  glMultiTexCoord2iPROC glMultiTexCoord2i;
  glMultiTexCoord2ivPROC glMultiTexCoord2iv;
  glMultiTexCoord2sPROC glMultiTexCoord2s;
  glMultiTexCoord2svPROC glMultiTexCoord2sv;
  glMultiTexCoord3dPROC glMultiTexCoord3d;
  glMultiTexCoord3dvPROC glMultiTexCoord3dv;
  glMultiTexCoord3fPROC glMultiTexCoord3f;
  glMultiTexCoord3fvPROC glMultiTexCoord3fv;
  glMultiTexCoord3iPROC glMultiTexCoord3i;
  glMultiTexCoord3ivPROC glMultiTexCoord3iv;
  glMultiTexCoord3sPROC glMultiTexCoord3s;
  glMultiTexCoord3svPROC glMultiTexCoord3sv;
  glMultiTexCoord4dPROC glMultiTexCoord4d;
  glMultiTexCoord4dvPROC glMultiTexCoord4dv;
  glMultiTexCoord4fPROC glMultiTexCoord4f;
  glMultiTexCoord4fvPROC glMultiTexCoord4fv;
  glMultiTexCoord4iPROC glMultiTexCoord4i;
  glMultiTexCoord4ivPROC glMultiTexCoord4iv;
  glMultiTexCoord4sPROC glMultiTexCoord4s;
  glMultiTexCoord4svPROC glMultiTexCoord4sv;
  
  //ARB_shader_objects
  int ARB_shader_objects;
  glDeleteObjectARBPROC glDeleteObjectARB;
  glGetHandleARBPROC glGetHandleARB;
  glDetachObjectARBPROC glDetachObjectARB;
  glCreateShaderObjectARBPROC glCreateShaderObjectARB;
  glShaderSourceARBPROC glShaderSourceARB;
  glCompileShaderARBPROC glCompileShaderARB;
  glCreateProgramObjectARBPROC glCreateProgramObjectARB;
  glAttachObjectARBPROC glAttachObjectARB;
  glLinkProgramARBPROC glLinkProgramARB;
  glUseProgramObjectARBPROC glUseProgramObjectARB;
  glValidateProgramARBPROC glValidateProgramARB;
  glUniform1fARBPROC glUniform1fARB;
  glUniform2fARBPROC glUniform2fARB;
  glUniform3fARBPROC glUniform3fARB;
  glUniform4fARBPROC glUniform4fARB;
  glUniform1iARBPROC glUniform1iARB;
  glUniform2iARBPROC glUniform2iARB;
  glUniform3iARBPROC glUniform3iARB;
  glUniform4iARBPROC glUniform4iARB;
  glUniform1fvARBPROC glUniform1fvARB;
  glUniform2fvARBPROC glUniform2fvARB;
  glUniform3fvARBPROC glUniform3fvARB;
  glUniform4fvARBPROC glUniform4fvARB;
  glUniform1ivARBPROC glUniform1ivARB;
  glUniform2ivARBPROC glUniform2ivARB;
  glUniform3ivARBPROC glUniform3ivARB;
  glUniform4ivARBPROC glUniform4ivARB;
  glUniformMatrix2fvARBPROC glUniformMatrix2fvARB;
  glUniformMatrix3fvARBPROC glUniformMatrix3fvARB;
  glUniformMatrix4fvARBPROC glUniformMatrix4fvARB;
  glGetObjectParameterfvARBPROC glGetObjectParameterfvARB;
  glGetObjectParameterivARBPROC glGetObjectParameterivARB;
  glGetInfoLogARBPROC glGetInfoLogARB;
  glGetAttachedObjectsARBPROC glGetAttachedObjectsARB;
  glGetUniformLocationARBPROC glGetUniformLocationARB;
  glGetActiveUniformARBPROC glGetActiveUniformARB;
  glGetUniformfvARBPROC glGetUniformfvARB;
  glGetUniformivARBPROC glGetUniformivARB;
  glGetShaderSourceARBPROC glGetShaderSourceARB;

  //EXT_framebuffer_object
  int EXT_framebuffer_object;
  glBindFramebufferEXTPROC glBindFramebufferEXT;
  glBindRenderbufferEXTPROC glBindRenderbufferEXT;
  glCheckFramebufferStatusEXTPROC glCheckFramebufferStatusEXT;
  glDeleteFramebuffersEXTPROC glDeleteFramebuffersEXT;
  glDeleteRenderBuffersEXTPROC glDeleteRenderBuffersEXT;
  glFramebufferRenderbufferEXTPROC glFramebufferRenderbufferEXT;
  glFramebufferTexture1DEXTPROC glFramebufferTexture1DEXT;
  glFramebufferTexture2DEXTPROC glFramebufferTexture2DEXT;
  glFramebufferTexture3DEXTPROC glFramebufferTexture3DEXT;
  glGenFramebuffersEXTPROC glGenFramebuffersEXT;
  glGenRenderbuffersEXTPROC glGenRenderbuffersEXT;
  glGenerateMipmapEXTPROC glGenerateMipmapEXT;
  glGetFramebufferAttachmentParameterivEXTPROC glGetFramebufferAttachmentParameterivEXT;
  glGetRenderbufferParameterivEXTPROC glGetRenderbufferParameterivEXT;
  glIsFramebufferEXTPROC glIsFramebufferEXT;
  glIsRenderbufferEXTPROC glIsRenderbufferEXT;
  glRenderbufferStorageEXTPROC glRenderbufferStorageEXT;

#ifdef _WIN32
  int WGL_EXT_swap_control;
  wglSwapIntervalEXTPROC wglSwapIntervalEXT;
  wglGetSwapIntervalEXTPROC wglGetSwapIntervalEXT;
#endif

private:
  void *GetProcAddress(char *);
  
  void InitMultitexture();
  void InitARBShaderObjects();
  void InitEXTFramebufferObject();

#ifdef _WIN32  
  void InitWGLEXTSwapControl();
#endif
};

#endif

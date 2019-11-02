/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OMNIC_GL_TYPES_H_
#define OMNIC_GL_TYPES_H_

#include <vector>

// Use Qt QOpenGLFunctions when USE_QT_GL is enabled
#include <QOpenGLFunctions>
#define OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS : protected QOpenGLFunctions

// Use standard gl functions

#ifdef __APPLE__
#include <OpenGL/GL.h>
#endif 

#ifdef __LINUX__
#include <GL/GL.h>
#endif 



namespace omnic
{
  namespace gl
  {
    /// Returns the type id of a data type in constexpr typeId() function
    template<typename T>
    struct TypeId
    {
    };

    /// Returns the internal type id of given format and data type in constexpr typeId() function
    template<GLuint FORMAT, typename T>
    struct PixelTypeId
    {
    };

    /// GL_RGBA + GLfloat -> GL_RGBA32F
    template<>
    struct PixelTypeId<GL_RGBA,GLfloat>
    {
      inline static constexpr GLuint typeId()
      {
        return GL_RGBA32F;
      }
    };

    /// GL_RGBA + GLushort -> GL_RGBA16
    template<>
    struct PixelTypeId<GL_RGBA,GLushort>
    {
      inline static constexpr GLuint typeId()
      {
        return GL_RGBA16;
      }
    };

    namespace traits {
      /// Traits for width member function, e.g. for buffer types
      template<typename T>
      struct Width {
        template<typename BUF>
        int operator()(BUF const& _buf) {
          return _buf.width();
        }
      };
      
      /// Traits for height member function, e.g. for buffer types
      template<typename T>
      struct Height {
        template<typename BUF>
        int operator()(BUF const& _buf) {
          return _buf.height();
        }
      };
      
      /// Traits for ptr member function, e.g. for buffer types
      template<typename T>
      struct Ptr {
        template<typename BUF>
        void const* operator()(BUF const& _buf) {
          return _buf.ptr();
        }
      };
      
      template<typename T>
      struct Width<std::vector<T>> {
        template<typename BUF>
        int operator()(BUF const& _buf) {
          return _buf.size();
        }
      };
      
      template<typename T>
      struct Height<std::vector<T>> {
        template<typename BUF>
        int operator()(BUF const&) {
          return 1;
        }
      };

      template<typename T>
      struct Ptr<std::vector<T>> {
        template<typename BUF>
        void const* operator()(BUF const& _buf) {
          return (void const*)_buf.data();
        }
      };

      /// Return width of object of type T
      template<typename T>
      int width(T const& _t) { return Width<T>()(_t); };      

      /// Return height of object of type T
      template<typename T>
      int height(T const& _t) { return Height<T>()(_t); };

      /// Return const void* pointer of object of type T
      template<typename T>
      void const* ptr(T const& _t) { return Ptr<T>()(_t); };      
    }
  }
}

#define OMNIC_GL_TYPEID(TYPE,ID) \
      namespace omnic { namespace gl {\
      template<> \
      struct TypeId<TYPE> { \
        inline static constexpr GLuint typeId() { return ID; } \
      }; \
      }}

OMNIC_GL_TYPEID(GLfloat,GL_FLOAT)
OMNIC_GL_TYPEID(GLushort,GL_UNSIGNED_SHORT)
OMNIC_GL_TYPEID(GLuint,GL_UNSIGNED_INT)
OMNIC_GL_TYPEID(GLubyte,GL_UNSIGNED_BYTE)

#endif /* OMNIC_GL_TYPES_H_ */

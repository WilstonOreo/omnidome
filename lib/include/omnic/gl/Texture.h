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

#ifndef OMNIC_GL_TEXTURE_H_
#define OMNIC_GL_TEXTURE_H_

#include <omnic/util.h>
#include <omnic/gl/functions.h>
#include <omnic/gl/TextureRef.h>

namespace omnic {
  namespace gl {
    /**@brief Template class for holding a GL_TEXTURE_2D with arbitrary data type
     * @tparam 
     **/
    template<GLuint FORMAT, typename T> 
    class Texture OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS {
    public:
      /// Our channel type 
      typedef T type;

      /// GLuint of format id
      inline static constexpr GLuint format() { return FORMAT; }

      /// Default constructor 
      Texture() {
      }
      
      /// Construct from buffer type
      template<typename BUF>
      Texture(BUF const& _buf, GLuint _target = GL_TEXTURE_2D) {
        initialize(_buf,_target);
      }

      /// Destruct and destroy texture
      ~Texture() {
        destroy();
      }

      /// Initialize texture data from buffer
      template<typename BUF>
      void initialize(BUF const& _buf, GLuint _target = GL_TEXTURE_2D) {
#if OMNIC_USE_QT_GL 
          initializeOpenGLFunctions();
#endif
        destroy();

        GLuint _texId;
        glGenTextures(1, &_texId);
        tex_ = TextureRef(_texId,traits::width(_buf),traits::height(_buf),_target);
        
        OMNIC_ASSERT(glGetError() == GL_NO_ERROR);

        bind();
        {
          gl::texImage<format(),type>(target(),_buf);
          glTexParameteri(target(), GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
          glTexParameteri(target(), GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
          glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        release();
      }

      /// Destroy texture and set internal tex ID to 0
      void destroy() {
        auto _texId = tex_.id();
        if (_texId != 0) {
          glDeleteTextures(1, &_texId);
          tex_ = TextureRef();
        } 
      }

      /// Bind texture with glBindTexture(target,texId)
      void bind() {
        glBindTexture(target(), textureId());
      }

      /// Release texture with glBindTexture(target,0)
      void release() {
        glBindTexture(target(), 0);
      }

      /// Return target (e.g. GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE)
      GLuint target() const {
        return tex_.target();
      }

      /// Return id of this texture
      GLuint textureId() const {
        return tex_.id();
      }

      /// Return width of texture
      uint32_t width() const {
        return tex_.width();
      }
      
      /// Return width of texture
      uint32_t height() const {
        return tex_.height();
      }

    private:
      TextureRef tex_;
    };

    typedef Texture<GL_RGBA,GLfloat> TextureRGBA32F;
    typedef Texture<GL_RGBA,GLushort> TextureRGBA16;
  }
}

#endif /* OMNIC_GL_TEXTURE_H_ */

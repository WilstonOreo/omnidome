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

#ifndef OMNIC_GL_TEXTUREREF_H_
#define OMNIC_GL_TEXTUREREF_H_

#include <omnic/gl/types.h>

namespace omnic {
  namespace gl {
    /// Reference to texture with id, width, height and target
    struct TextureRef { 
      /**@brief Constructor with texture id, width, height and optional target
         @param _textureId OpenGL id of texture
         @param _textureWidth Width (in pixels) of texture
         @param _textureHeight Height (in pixels) of texture
         @param _target Texture target (GL_TEXTURE_2D by default)
       **/
      TextureRef(GLuint _id = 0,
          uint32_t _width = 0,
          uint32_t _height = 0,
          GLuint _target = GL_TEXTURE_2D) :
        id_(_id),
        width_(_width),
        height_(_height),
        target_(_target) {}

      /// Construct from object like QOpenGLTexture
      template<typename T>
      TextureRef(T const& _t) :
        id_(_t.textureId()),
        width_(_t.width()),
        height_(_t.height()),
        target_(_t.target()) {
      }

      inline GLuint id() const {
        return id_;
      }

      inline uint32_t width() const {
        return width_;
      }

      inline uint32_t height() const {
        return height_;
      }

      inline GLuint target() const {
        return target_;
      }

    private:
      GLuint id_ = 0;
      uint32_t width_;
      uint32_t height_;
      GLuint target_;
    };
  }
}

#endif /* OMNIC_GL_TEXTUREREF_H_ */

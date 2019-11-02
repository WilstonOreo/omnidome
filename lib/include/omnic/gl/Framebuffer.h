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

#ifndef OMNIC_GL_FRAMEBUFFER_H_
#define OMNIC_GL_FRAMEBUFFER_H_

#include <omnic/gl/functions.h>

namespace omnic {
  namespace gl {

    /// Framebuffer with compile time format and pixel type
    template<GLuint FORMAT, typename T>  
    class Framebuffer OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS {
      public:
        Framebuffer();
        Framebuffer(int _width, int _height) {
          initialize(_width,_height);
        }

        ~Framebuffer() {
          destroy();
        }

        /// Return width of framebuffer
        int width() const {
          return width_;
        }

        /// Return height of framebuffer
        int height() const {
          return height_;
        }

        /// Initialize with width and height
        void initialize(int _w, int _h) {
          initializeOpenGLFunctions();

          if (width_ == _w && height_ == _h) return;

          destroy();

          // 2D texture to render to
          glGenTextures(1, &colorTex_);
          glBindTexture(GL_TEXTURE_2D, colorTex_);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

          // NULL means reserve texture memory, but texels are undefined
          gl::texImage<FORMAT,T>(GL_TEXTURE_2D,_w,_h);

          // -------------------------
          glGenFramebuffers(1, &fb_);
          bind();

          //   Attach 2D texture to this FBO
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, colorTex_, 0);


          // -------------------------
          glGenRenderbuffers(1, &depthRb_);
          glBindRenderbuffer(GL_RENDERBUFFER, depthRb_);
          glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _w,_h);

          // -------------------------
          // Attach depth buffer to FBO
          glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                    GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER, depthRb_);

          // -------------------------
          // Does the GPU support current FBO configuration?
          GLenum status;
          status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

          if (status != GL_FRAMEBUFFER_COMPLETE) {
            destroy();
            return;
          }
          width_ = _w;
          height_ = _h;
          glBindTexture(GL_TEXTURE_2D,0);
          glBindRenderbuffer(GL_RENDERBUFFER,0);
          release();
        }

        GLuint texture() const {  
          return colorTex_;
        }

        void bind() {
          glBindFramebuffer(GL_FRAMEBUFFER, fb_);
        }

        void release() {
          glBindFramebuffer(GL_FRAMEBUFFER, 0 /* \@todo bind to default here */ );
        }

        /// Destroy framebuffer from current context
        void destroy() {
          if (colorTex_) glDeleteTextures(1, &colorTex_);
          if (depthRb_) glDeleteRenderbuffers(1, &depthRb_);
          if (fb_) glDeleteFramebuffers(1, &fb_);
          width_ = 0; height_ = 0;
        }

      private:
        int width_ = 0;
        int height_ = 0;
        GLuint fb_ = 0;
        GLuint colorTex_ = 0;
        GLuint depthRb_ = 0;
      };

    typedef Framebuffer<GL_RGBA,GLushort> FramebufferRGBA16;
    typedef Framebuffer<GL_RGBA,GLfloat> FramebufferRGBA32F;
  }
}

#endif /* OMNIC_GL_FRAMEBUFFER_H_ */


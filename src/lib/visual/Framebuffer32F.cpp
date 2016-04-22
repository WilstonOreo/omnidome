/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <omni/visual/Framebuffer32F.h>

#include <QOpenGLFramebufferObject>

namespace omni {
  namespace visual {
    Framebuffer32F::Framebuffer32F() {

    }
    Framebuffer32F::Framebuffer32F(QSize const& _size) {
      initialize(_size);
    }

    Framebuffer32F::~Framebuffer32F() {
      free();
    }

    void Framebuffer32F::initialize(QSize const& _size) {
      if (_size == size()) return;

      free();
      visual::with_current_context([&](QOpenGLFunctions& _) {
        int _h = _size.height();
        int _w = _size.width();
        // RGBA32F 2D texture to render to
        _.glGenTextures(1, &colorTex_);
        _.glBindTexture(GL_TEXTURE_2D, colorTex_);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // NULL means reserve texture memory, but texels are undefined
        _.glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA32F , _w, _h, 0, GL_RGBA,
                       GL_FLOAT, NULL);

        // -------------------------
        _.glGenFramebuffers(1, &fb_);
        bind();

        //   Attach 2D texture to this FBO
        _.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, colorTex_, 0);


        // -------------------------
        _.glGenRenderbuffers(1, &depthRb_);
        _.glBindRenderbuffer(GL_RENDERBUFFER, depthRb_);
        _.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _w,
                                _h);

        // -------------------------
        // Attach depth buffer to FBO
        _.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                    GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER, depthRb_);

        // -------------------------
        // Does the GPU support current FBO configuration?
        GLenum status;
        status = _.glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
          free();
          return;
        }
        size_ = QSize(_w,_h);
        _.glBindTexture(GL_TEXTURE_2D,0);
        _.glBindRenderbuffer(GL_RENDERBUFFER,0);
        release();
      });
    }

    void Framebuffer32F::free() {
      visual::with_current_context([&](QOpenGLFunctions& _) {
        if (colorTex_) _.glDeleteTextures(1, &colorTex_);
        if (depthRb_) _.glDeleteRenderbuffers(1, &depthRb_);
        if (fb_) _.glDeleteFramebuffers(1, &fb_);
        size_ = QSize(0,0);
      });
    }

    void Framebuffer32F::bind() {
      visual::with_current_context([&](QOpenGLFunctions& _) {
        _.glBindFramebuffer(GL_FRAMEBUFFER, fb_);
      });
    }

    void Framebuffer32F::release() {
      visual::with_current_context([&](QOpenGLFunctions& _) {
        QOpenGLFramebufferObject::bindDefault();
      });
    }

    GLuint Framebuffer32F::texture() const {
      return colorTex_;
    }

    int Framebuffer32F::width() const {
      return size().width();
    }

    int Framebuffer32F::height() const {

      return size().height();
    }

    QSize const& Framebuffer32F::size() const {
      return size_;
    }
  }
}

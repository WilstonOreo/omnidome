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

#include <omni/visual/Texture32F.h>

#include <omni/visual/util.h>

namespace omni {
  namespace visual {
    Texture32F::Texture32F() {}

    Texture32F::Texture32F(RenderBuffer const& _buf, GLuint _target) {
      initialize(_buf, _target);
    }

    Texture32F::~Texture32F() {
      destroy();
    }

    void Texture32F::destroy() {
      if (texId_ != 0) {
        with_current_context([&](QOpenGLFunctions& _) {
          _.glDeleteTextures(1, &texId_);
          texId_ = 0;
        });
      }
    }

    GLuint Texture32F::target() const {
      return target_;
    }

    GLuint Texture32F::textureId() const {
      return texId_;
    }

    void Texture32F::initialize(RenderBuffer const& _buf, GLuint _target) {
      destroy();
      target_ = _target;
      size_= QSize(_buf.width(), _buf.height());
      with_current_context([&](QOpenGLFunctions& _) {
        _.glGenTextures(1, &texId_);
        _.glBindTexture(target_, texId_);
        _.glTexParameteri(target_, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(target_, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        _.glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _.glTexImage2D(target_, 0, GL_RGBA32F,
                       size_.width(),
                       size_.height(), 0,
                       GL_RGBA, GL_FLOAT, _buf.ptr());
        _.glBindTexture(target_, 0);
      });
    }

    void Texture32F::bind() {
      with_current_context([&](QOpenGLFunctions& _) {
        _.glBindTexture(target_, texId_);
      });
    }

    void Texture32F::release() {
      with_current_context([&](QOpenGLFunctions& _) {
        _.glBindTexture(target_, 0);
      });
    }
  }
}

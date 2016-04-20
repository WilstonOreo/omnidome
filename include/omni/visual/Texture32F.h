
/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
#ifndef OMNI_VISUAL_TEXTURE32F_H_
#define OMNI_VISUAL_TEXTURE32F_H_

#include <QOpenGLFunctions>
#include <omni/RenderBuffer.h>

namespace omni {
  namespace visual {

    /// RGBA Texture with 32-bit float for each channel
    class Texture32F {
    public:
      Texture32F();

      /// Construct from render buffer and initialize
      Texture32F(RenderBuffer const& _buf, GLuint _target = GL_TEXTURE_2D);

      ~Texture32F();

      /// Initialize texture data from render buffer
      void initialize(RenderBuffer const& _buf, GLuint _target = GL_TEXTURE_2D);

      void destroy();

      /// Bind texture with glBindTexture(target,texId)
      void bind();

      /// Release texture with glBindTexture(target,0)
      void release();

      /// Return GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE
      GLuint target() const;

      /// Return id of this texture
      GLuint textureId() const;

    private:
      QSize size_;
      GLuint target_ = GL_TEXTURE_2D;
      GLuint texId_ = 0;
    };

  }
}

#endif /* OMNI_VISUAL_TEXTURE32F_H_ */

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
#ifndef OMNI_VISUAL_FRAMEBUFFER32F_H_
#define OMNI_VISUAL_FRAMEBUFFER32F_H_

#include <omni/visual/util.h>

namespace omni {
  namespace visual {
      /// Framebuffer with 32-bit float pixel depth per channel
      class Framebuffer32F {
      public:
        Framebuffer32F();
        Framebuffer32F(QSize const&);
        ~Framebuffer32F();

        /// Return width of framebuffer
        int width() const;

        /// Return height of framebuffer
        int height() const;

        /// Return size of framebuffer
        QSize const& size() const;

        void initialize(QSize const&);

        GLuint texture() const;

        void bind();

        void release();

        /// Free buffer from current context
        void destroy();

      private:

        QSize size_;
        GLuint fb_ = 0;
        GLuint colorTex_ = 0;
        GLuint depthRb_ = 0;
      };
  }
}

#endif /* OMNI_VISUAL_FRAMEBUFFER32F_H_ */

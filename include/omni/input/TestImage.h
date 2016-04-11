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

#ifndef OMNI_INPUT_TESTIMAGE_H_
#define OMNI_INPUT_TESTIMAGE_H_

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include "Interface.h"

namespace omni {
  namespace input {
    /**@brief Abstract class for test images that are generated with a shader
    **/
    class TestImage :
      public Interface {
      public:
        TestImage(Interface const *_parent = nullptr);
        virtual ~TestImage();

        /// Texture id is tex id of internal framebuffer
        GLuint       textureId() const;

        /// Free framebuffer
        void         free();

        /// Update framebuffer image
        virtual void update();

        /// Return ruler position
        QPointF      rulerPos() const;

        /// Set ruler position
        void         setRulerPos(QPointF const&);

        /// Serialize to stream
        virtual void toStream(QDataStream&) const;

        /// Deserialize from stream
        virtual void fromStream(QDataStream&);

      protected:
        /**@brief Virtual method to handle additional shader uniforms in derived classes
         **/
        virtual void shaderUniformHandler();

        std::unique_ptr<QOpenGLShaderProgram> shader_;

      private:
        /// String representing the fragment shader source
        virtual QString fragmentShaderSource() const = 0;

        /// String representing the vertex shader source
        virtual QString vertexShaderSource() const   = 0;

        QPointF rulerPos_;
        std::unique_ptr<QOpenGLFramebufferObject> framebuffer_;
    };
  }
}

#endif /* OMNI_INPUT_TESTIMAGE_H_ */

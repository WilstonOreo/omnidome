/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
#ifndef OMNI_VISUAL_UNIFORMHANDLER_H_
#define OMNI_VISUAL_UNIFORMHANDLER_H_

#include <set>
#include <functional>
#include <QOpenGLShaderProgram>
#include <omni/global.h>

class QOpenGLFunctions;

namespace omni {
  namespace visual {
    /**@brief Uniform handler for a shader program
       @detail Cares about bind and releasing shaders and texture uniforms.
               Should only be used with the visual::useShader function.
     **/
    struct OMNI_EXPORT UniformHandler {
      UniformHandler(QOpenGLFunctions& _gl,
                     QOpenGLShaderProgram& _shader);

      /// Destructor. Unbind all active texture units
      ~UniformHandler();

      /// Set uniform value
      template<typename ... ARGS>
      void uniform(const char *_name, ARGS&& ... _args) {
        shader_.setUniformValue(_name, _args ...);
      }

      /// Set uniform value for texture with id and optional target
      void texUniform(const char *_name,
                      GLuint _texId,
                      GLuint _target = GL_TEXTURE_2D);

      /**@brief Set uniform value for texture object
         @detail Texture object can be either QOpenGLTexture or Texture32F
       **/
      template<typename TEXTURE>
      void texUniform(const char *_name, TEXTURE& _tex) {
        texUniform(_name, _tex.textureId(), _tex.target());
      }

      void texRectUniform(const char* _name, GLuint _texId, QSize _size);  

      private:
        QOpenGLFunctions    & gl_;
        QOpenGLShaderProgram& shader_;
        GLint                 currentTextureUnit_ = 0;
        std::set<GLuint>      usedTextureTargets_;
    };
  }
}

#endif /* OMNI_VISUAL_UNIFORMHANDLER_H_ */

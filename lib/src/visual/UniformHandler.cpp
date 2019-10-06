
/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include <omni/visual/UniformHandler.h>

#include <omni/visual/util.h>

#include <QOpenGLFunctions>

namespace omni {
  namespace visual {
    UniformHandler::UniformHandler(QOpenGLFunctions& _gl,
                                   QOpenGLShaderProgram& _shader) :
      gl_(_gl),
      shader_(_shader) {}

    UniformHandler::~UniformHandler() {
      gl_.glActiveTexture(GL_TEXTURE0);

      for (auto& _usedTextureTarget : usedTextureTargets_) {
        gl_.glBindTexture(_usedTextureTarget, 0);
      }
    }

    void UniformHandler::texRectUniform(const char* _name, GLuint _texId, QSize _size) {
      texUniform(_name,_texId,GL_TEXTURE_RECTANGLE);
      uniform(
        QString(QString(_name) + "_size").toUtf8().data(),
        QVector2D(_size.width(),_size.height()));
    }

    void UniformHandler::texUniform(const char *_name,
                                    GLuint _texId,
                                    GLuint _target) {
      uniform(_name, currentTextureUnit_);
      gl_.glActiveTexture(GL_TEXTURE0 + currentTextureUnit_);
      gl_.glBindTexture(_target, _texId);
      usedTextureTargets_.insert(_target);
      ++currentTextureUnit_;
    }
  }
}

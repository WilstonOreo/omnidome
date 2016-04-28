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

#include <omni/visual/Shader.h>

namespace omni {
  namespace visual {
    void initShader(QOpenGLShaderProgram& _s, const char *_filename) {
      QString _vertSrc =
        omni::util::fileToStr(":/shaders/" + QString(_filename) + ".vert");
      QString _fragmentSrc =
        omni::util::fileToStr(":/shaders/" + QString(_filename) + ".frag");

      _s.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                 _vertSrc);
      _s.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                 _fragmentSrc);
      _s.link();
    }

    void initShader(std::unique_ptr<QOpenGLShaderProgram>& _s,
                    const char *_filename) {
      /// Dont do anything if shader is already allocated
      if (!!_s) return;

      _s.reset(new QOpenGLShaderProgram());
      initShader(*_s, _filename);
    }

    void initShader(ContextBoundPtr<QOpenGLShaderProgram>& _s,
                    const char *_filename) {
      /// Dont do anything if shader is already allocated
      if (!!_s) return;

      if (_s.reset(new QOpenGLShaderProgram())) {
        initShader(*_s, _filename);
      }
    }

    void useShader(QOpenGLShaderProgram& _s,
                   std::function<void(UniformHandler&)>f) {
      withCurrentContext([&](QOpenGLFunctions& _gl) {
        _s.bind();
        {
          UniformHandler _handler(_gl, _s);
          f(_handler);

          // Destructor of handler is called here implicitly
          // by RAII to unbind all textures
        }
        _s.release();
      });
    }
  }
}


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

#include <omni/visual/util.h>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <string>
#include <chrono>

namespace omni {
  namespace visual {
    namespace util {
      /// Get current time in nano seconds
      double now() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count()
               /
               1000000000.0;
      }

      void checkOpenGLError() {
        withCurrentContext([&](QOpenGLFunctions& _) {
          GLenum err(_.glGetError());

          while (err != GL_NO_ERROR) {
            std::string error = "GL_";

            switch (err) {
            case GL_INVALID_OPERATION:      error += "INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:           error += "INVALID_ENUM";           break;
            case GL_INVALID_VALUE:          error += "INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:          error += "OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error += "INVALID_FRAMEBUFFER_OPERATION";  break;
            }

            OMNI_DEBUG << error.c_str();
            err = _.glGetError();
          }
        });
      }

      QRectF viewRect(int _imageWidth,
                    int _imageHeight,
                    int _canvasWidth,
                    int _canvasHeight,
                    float _border) {
        float _projAspect = float(_imageWidth) /
                            _imageHeight;
        float _viewAspect = float(_canvasWidth) / _canvasHeight;
        float b           = _border * 0.5;
        float _left       = -0.5 - b, _right = 0.5 + b, _bottom = -0.5 - b,
              _top        = 0.5 + b;

        if (_projAspect > _viewAspect)
        {
          _top    *= _projAspect / _viewAspect;
          _bottom *=  _projAspect / _viewAspect;
        }
        else
        {
          _left  *= _viewAspect / _projAspect;
          _right *= _viewAspect / _projAspect;
        }

        return QRectF(QPointF(_left, _top), QPointF(_right, _bottom));
      }

      void resetOpenGLState() {
        withCurrentContext([&](QOpenGLFunctions& _gl) {
          _gl.glBindBuffer(GL_ARRAY_BUFFER, 0);
          _gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

          if (QOpenGLContext::currentContext()->isOpenGLES() ||
              (_gl.openGLFeatures() & QOpenGLFunctions::FixedFunctionPipeline)) {
            int maxAttribs;
            _gl.glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

            for (int i = 0; i < maxAttribs; ++i) {
              _gl.glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 0, 0);
              _gl.glDisableVertexAttribArray(i);
            }
          }
          _gl.glActiveTexture(GL_TEXTURE0);
          _gl.glBindTexture(GL_TEXTURE_2D, 0);
          _gl.glDisable(GL_DEPTH_TEST);
          _gl.glDisable(GL_STENCIL_TEST);
          _gl.glDisable(GL_SCISSOR_TEST);
          _gl.glDisable(GL_CULL_FACE);
          _gl.glColorMask(true, true, true, true);
          _gl.glClearColor(0, 0, 0, 1);

          _gl.glDepthMask(true);
          _gl.glDepthFunc(GL_LESS);
          _gl.glClearDepthf(1);

          _gl.glStencilMask(0xff);
          _gl.glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
          _gl.glStencilFunc(GL_ALWAYS, 0, 0xff);
          _gl.glUseProgram(0);
          _gl.glEnable(GL_DEPTH_TEST);
          _gl.glDepthFunc(GL_LEQUAL);
          _gl.glEnable(GL_BLEND);
          _gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          _gl.glEnable(GL_LINE_SMOOTH);
          _gl.glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
          _gl.glEnable(GL_POINT_SMOOTH);
          _gl.glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
          _gl.glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
          _gl.glEnable(GL_NORMALIZE);

          // fix outlines z-fighting with quads
          _gl.glPolygonOffset(1, 1);
          QOpenGLFramebufferObject::bindDefault();
        });
      }
    }
  }
}

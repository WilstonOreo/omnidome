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

#ifndef OMNI_VISUAL_UTIL_H_
#define OMNI_VISUAL_UTIL_H_

#include <math.h>
#include <QDebug>
#include <QPointF>
#include <QRectF>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <omni/util.h>
#include <omni/visual/ContextBoundPtr.h>

class QOpenGLShaderProgram;

namespace omni {
  namespace visual {
    namespace util {
      /// Get current time in nano seconds
      double now();


      template<typename F>
      void for_each_circle_point(size_t _numVertices, float _radius, F _f)
      {
        for (size_t i = 0; i < _numVertices; ++i)
        {
          float _m   = 2.0 * M_PI * float(i) / _numVertices;
          float _cos = cos(_m), _sin = sin(_m);
          _f(i, QPointF(_cos * _radius, _sin * _radius));
        }
      }

      /// Do OpenGL operations with current context, if it exists
      void with_current_context(std::function<void(QOpenGLFunctions&)> f);

      /// Initialize shader: load from file and compile.
      void initShader(QOpenGLShaderProgram& _s, const char* _filename);

      /// Initialize shader: load from file and compile.
      void initShader(std::unique_ptr<QOpenGLShaderProgram>& _s, const char* _filename);

      /// Initialize shader: load from file and compile.
      void initShader(ContextBoundPtr<QOpenGLShaderProgram>& _s, const char* _filename);

      /// Calculate view rectangle on 2D OpenGL surface
      QRectF viewRect(int _imageWidth,
                                    int _imageHeight,
                                    int _canvasWidth,
                                    int _canvasHeight,
                                    float _border = 0.0);

      template<typename F>
      void for_each_arc_point(size_t _numVertices,
                              float _radius,
                              float _beginAngle,
                              float _endAngle,
                              F _f)
      {
        float  _cos = cos(_beginAngle), _sin = sin(_beginAngle);
        size_t i    = 0;

        _f(0, QPointF(_cos * _radius, _sin * _radius));

        for (; i < _numVertices; ++i)
        {
          float _m =  _beginAngle + float(i) / _numVertices *
                     (_endAngle - _beginAngle);
          _cos = cos(_m), _sin = sin(_m);
          _f(i, QPointF(_cos * _radius, _sin * _radius));
        }
        _cos = cos(_endAngle), _sin = sin(_endAngle);
        _f(i, QPointF(_cos * _radius, _sin * _radius));
      }


      /// Calculates the aspect ratio from a QSize
      template<typename SIZE>
      qreal aspect(SIZE const& _size)
      {
        return _size.width() / qreal(_size.height());
      }

      /// Reset openGL state to its defaults
      void resetOpenGLState();


      /// Convenience function for handling glGetError()
      void checkOpenGLError();

      /// Draw into QOpenGLFramebufferObject with given projection and model
      // view operations
      template<typename FRAMEBUFFER, typename PROJECTION, typename MODELVIEW>
      void draw_on_framebuffer(FRAMEBUFFER* _f, PROJECTION _p, MODELVIEW _m)
      {
        with_current_context([&](QOpenGLFunctions& _) {
          _f->bind();
          _.glViewport(0, 0, _f->width(), _f->height());
          _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);

          glMatrixMode(GL_TEXTURE);
          glLoadIdentity();

          // Projection matrix setup
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          _p(_); // Projection operation

          // Model view matrix setup
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();

          _m(_); // ModelView operation

          _f->release();
        });
      }

      /// Set viewport for widget
      template<typename WIDGET>
      void viewport(WIDGET *_widget)
      {
        with_current_context([&_widget](QOpenGLFunctions& _)
        {
          int d = _widget->devicePixelRatio();
          _.glViewport(0, 0, _widget->width() * d, _widget->height() * d);
        });
      }
    }

    using util::checkOpenGLError;
    using util::resetOpenGLState;
    using util::initShader;
    using util::with_current_context;
    using util::draw_on_framebuffer;
    using util::viewport;
  }
}


#endif /* OMNI_VISUAL_UTIL_H_ */

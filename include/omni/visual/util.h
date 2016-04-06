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
#include <chrono>
#include <QDebug>
#include <QPointF>
#include <QRectF>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>


namespace omni {
  namespace visual {
    namespace util {
      /// Get current time in nano seconds
      inline static double now() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count() /
          1000000000.0;
      }

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

      static inline QRectF viewRect(int _imageWidth,
                                    int _imageHeight,
                                    int _canvasWidth,
                                    int _canvasHeight,
                                    float _border = 0.0) {
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

      /// Draw on current context, if it exists
      template<typename F>
      void with_current_context(F f)
      {
        auto _currentContext = QOpenGLContext::currentContext();

        if (!_currentContext) return;

        QOpenGLFunctions glFuncs(_currentContext);
        f(glFuncs);
      }

      /// Calculates the aspect ratio from a QSize
      template<typename SIZE>
      qreal aspect(SIZE const& _size)
      {
        return _size.width() / qreal(_size.height());
      }

      /// Draw into QOpenGLFramebufferObject with given projection and model
      // view operations
      template<typename FRAMEBUFFER, typename PROJECTION, typename MODELVIEW>
      void draw_on_framebuffer(FRAMEBUFFER& _f, PROJECTION _p, MODELVIEW _m)
      {
        with_current_context([&](QOpenGLFunctions& _) {
          glPushAttrib(GL_ALL_ATTRIB_BITS);
          glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

          _f->bind();
          _.glViewport(0, 0, _f->width(), _f->height());
          _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);

          _.glDepthFunc(GL_LEQUAL);

          // fix outlines z-fighting with quads
          _.glPolygonOffset(1, 1);

          glMatrixMode(GL_TEXTURE);
          glPushMatrix();
          glLoadIdentity();

          // Projection matrix setup
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          _p(_); // Projection operation

          _.glEnable(GL_DEPTH_TEST);

          // Model view matrix setup
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          _.glClearColor(0.0, 0.0, 0.0, 1.0);
          _.glActiveTexture(GL_TEXTURE0);
          glClientActiveTexture(GL_TEXTURE0);
          _.glClear(GL_DEPTH_BUFFER_BIT);
          _.glEnable(GL_TEXTURE_2D);
          _.glDisable(GL_LIGHTING);
          _.glDepthFunc(GL_LEQUAL);
          _.glEnable(GL_BLEND);
          _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          _.glEnable(GL_LINE_SMOOTH);
          _.glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
          _.glEnable(GL_POINT_SMOOTH);
          _.glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
          _.glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
          glPolygonMode(GL_FRONT, GL_FILL);
          glPolygonMode(GL_BACK, GL_FILL);
          _.glEnable(GL_NORMALIZE);

          // fix outlines z-fighting with quads
          _.glPolygonOffset(1, 1);
          _m(_); // ModelView operation

          _f->release();

          glPopAttrib();
          glPopClientAttrib();
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
    using util::with_current_context;
    using util::draw_on_framebuffer;
    using util::viewport;
  }
}


#endif /* OMNI_VISUAL_UTIL_H_ */

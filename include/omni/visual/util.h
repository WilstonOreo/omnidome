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
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLDebugLogger>
#include <omni/util.h>

namespace omni {
  namespace visual {
    namespace util {
      /// Get current time in nano seconds
      inline static double now() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count()
               /
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

      /// Do OpenGL operations with current context, if it exists
      template<typename F>
      void with_current_context(F f)
      {
        auto _currentContext = QOpenGLContext::currentContext();
        if (!_currentContext) return;

        QOpenGLFunctions glFuncs(_currentContext);
        f(glFuncs);
      }

      /// Initialize shader: load from file and compile.
      template<typename SHADER, typename FILENAME>
      static void initShader(SHADER& _s, FILENAME const& _filename) {
        if (!!_s) return;

        QString _vertSrc =
          omni::util::fileToStr(":/shaders/" + QString(_filename) + ".vert");
        QString _fragmentSrc =
          omni::util::fileToStr(":/shaders/" + QString(_filename) + ".frag");
        _s.reset(new QOpenGLShaderProgram());
        _s->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                    _vertSrc);
        _s->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                    _fragmentSrc);
        _s->link();
      }


      /// Uniform handler for a shader program
      struct UniformHandler {
        UniformHandler(QOpenGLFunctions& _gl, QOpenGLShaderProgram& _shader) :
          gl_(_gl),
          shader_(_shader) {}

        ~UniformHandler() {
          if (currentTextureUnit_ > 0) {
            gl_.glActiveTexture(GL_TEXTURE0);
          }
          for (auto& _usedTextureTarget : usedTextureTargets_) {
            gl_.glBindTexture(_usedTextureTarget, 0);
          }
        }

        /// Set uniform value
        template<typename...ARGS>
        void uniform(const char* _name, ARGS&&..._args) {
          shader_.setUniformValue(_name,_args...);
        }


        /// Set uniform value for texture with id and optional target
        void texUniform(const char* _name, int _texId, GLuint _target = GL_TEXTURE_2D) {
          ++currentTextureUnit_;
          uniform(_name,currentTextureUnit_);
          gl_.glActiveTexture(GL_TEXTURE0 + currentTextureUnit_);
          gl_.glBindTexture(_target, _texId);
          usedTextureTargets_.insert(_target);
        }

        /// Set uniform value for texture object
        void texUniform(const char* _name, QOpenGLTexture& _tex) {
          texUniform(_name,_tex.textureId(),_tex.target());
        }

      private:
        QOpenGLFunctions& gl_;
        QOpenGLShaderProgram& shader_;
        int currentTextureUnit_ = 0;
        std::set<GLuint> usedTextureTargets_;
      };

      template<typename F>
      void useShader(QOpenGLShaderProgram& _s, F f) {
        with_current_context([&](QOpenGLFunctions& _gl) {
          _s.bind();
          {
            UniformHandler _handler(_gl,_s);
            f(_handler);
            // Destructor of handler is called here implicitly
            // by RAII to unbind all textures
          }
          _s.release();
        });
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


      /// Calculates the aspect ratio from a QSize
      template<typename SIZE>
      qreal aspect(SIZE const& _size)
      {
        return _size.width() / qreal(_size.height());
      }

      static inline void resetOpenGLState() {
        with_current_context([&](QOpenGLFunctions& _gl) {
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
          _gl.glColorMask(true, true, true, true);
          _gl.glClearColor(0, 0, 0, 0);

          _gl.glDepthMask(true);
          _gl.glDepthFunc(GL_LESS);
          _gl.glClearDepthf(1);

          _gl.glStencilMask(0xff);
          _gl.glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
          _gl.glStencilFunc(GL_ALWAYS, 0, 0xff);
          _gl.glDisable(GL_BLEND);
          _gl.glBlendFunc(GL_ONE, GL_ZERO);
          _gl.glUseProgram(0);

          QOpenGLFramebufferObject::bindDefault();
        });
      }

      /// Draw into QOpenGLFramebufferObject with given projection and model
      // view operations
      template<typename FRAMEBUFFER, typename PROJECTION, typename MODELVIEW>
      void draw_on_framebuffer(FRAMEBUFFER& _f, PROJECTION _p, MODELVIEW _m)
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

          _.glEnable(GL_DEPTH_TEST);

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

    using util::UniformHandler;
    using util::resetOpenGLState;
    using util::initShader;
    using util::useShader;
    using util::with_current_context;
    using util::draw_on_framebuffer;
    using util::viewport;
  }
}


#endif /* OMNI_VISUAL_UTIL_H_ */

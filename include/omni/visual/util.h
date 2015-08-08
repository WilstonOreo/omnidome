#ifndef OMNI_VISUAL_UTIL_H_
#define OMNI_VISUAL_UTIL_H_

#include <math.h>
#include <QPointF>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>


namespace omni
{
  namespace visual
  {
    namespace util
    {
      template<typename V>
      QPointF map_spherical(const V& _normal)
      {
        QPointF _texCoord(
               atan2(-_normal.y(),_normal.x()) / (2.0*M_PI),
               acos(_normal.z()) / M_PI); 
        return _texCoord;
      }

      template<typename F>
      void for_each_circle_point(size_t _numVertices, float _radius, F _f)
      {
        for (size_t i = 0; i < _numVertices; ++i)
        {
          float _m = 2.0 * M_PI * float(i) / _numVertices;
          float _cos = cos(_m), _sin = sin(_m);
          _f(i,QPointF(_cos*_radius,_sin*_radius));
        }
      }

      template<typename F>
      void for_each_arc_point(size_t _numVertices, float _radius, float _beginAngle, float _endAngle, F _f)
      {
        float _cos = cos(_beginAngle), _sin = sin(_beginAngle);
        size_t i= 0;
        _f(0,QPointF(_cos*_radius,_sin*_radius));
        for (; i < _numVertices; ++i)
        {
          float _m =  _beginAngle + float(i) / _numVertices * (_endAngle - _beginAngle);
          _cos = cos(_m), _sin = sin(_m);
          _f(i,QPointF(_cos*_radius,_sin*_radius));
        }
        _cos = cos(_endAngle), _sin = sin(_endAngle);
        _f(i,QPointF(_cos*_radius,_sin*_radius));
      }

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
   
      template<typename PROJECTION, typename MODELVIEW>
      void draw_on_framebuffer()
      {
      }

      /// Draw into QOpenGLFramebufferObject
      template<typename FRAMEBUFFER, typename PROJECTION, typename MODELVIEW>
      void draw_on_framebuffer(FRAMEBUFFER& _f, PROJECTION _p, MODELVIEW _m)
      {
        with_current_context([&](QOpenGLFunctions& _) { 
          
          glPushAttrib(GL_ALL_ATTRIB_BITS);
          _f->bind();
          _.glViewport(0,0, _f->width(), _f->height());
          _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
          _.glEnable(GL_TEXTURE_2D);
  
          // Projection matrix setup
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          _p(_); // Projection operation

          // Model view matrix setup
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          _m(_); // ModelView operation

        _f->release();

        glPopAttrib();
          
      });
    }


      template<typename WIDGET>
      void viewport(WIDGET* _widget)
      {
        with_current_context([&_widget](QOpenGLFunctions& _)
      {
        int d = _widget->devicePixelRatio();
        _.glViewport(0,0, _widget->width()*d, _widget->height()*d);
      });
      
      }
    }
    using util::with_current_context;
    using util::draw_on_framebuffer;
    using util::viewport;
  }
}



#endif /* OMNI_VISUAL_UTIL_H_ */

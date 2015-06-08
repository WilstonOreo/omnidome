#ifndef OMNI_VISUAL_UTIL_H_
#define OMNI_VISUAL_UTIL_H_

#include <QPointF>
#include <QVector2D>
#include <QVector3D>

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
    }
  }
}

#endif /* OMNI_VISUAL_UTIL_H_ */

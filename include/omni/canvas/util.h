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

#ifndef OMNI_CANVAS_UTIL_HPP_
#define OMNI_CANVAS_UTIL_HPP_

#include <QVector3D>
#include <QPointF>

namespace omni {
  namespace canvas {
    namespace util {
      /// Generate a cone with vertices and indices
      template<typename VERTICES, typename INDICES>
      static void generateCone(size_t _slices,
                               float _zTop,
                               float _zBottom,
                               float _radius,
                               VERTICES& _vertices,
                               INDICES& indices)
      {
        size_t _startIndex = _vertices.size();

        QVector3D _p(0, 0, _zTop);

        _vertices.emplace_back(_p, _p.normalized(), QPointF(0.5, 0.0));

        for_each_circle_point(_slices, _radius, [&](size_t i, const QPointF& _p2)
        {
          _p = QVector3D(_p2.x(), _p2.y(), _zBottom);
          QVector3D _normal = _p.normalized();
          _vertices.emplace_back(_p, _normal,
                                 QPointF(float(i) / _slices,
                                         acos(_normal.z()) / M_PI));

          if (_zTop > _zBottom)
          {
            indices.push_back(_startIndex);
            indices.push_back(_startIndex + 1 + i);
            indices.push_back(_startIndex + 1 + (i + 1) % _slices);
          } else
          {
            indices.push_back(_startIndex + 1 + (i + 1) % _slices);
            indices.push_back(_startIndex + 1 + i);
            indices.push_back(_startIndex);
          }
        });
      }

      /// Generate a stack with top and bottom radius
      template<typename VERTICES, typename INDICES>
      static void generateSphereStack(size_t _slices,
                                      float _zTop, float _zBottom,
                                      float _topRadius, float _bottomRadius,
                                      VERTICES& _vertices, INDICES& indices)
      {
        size_t _startIndex = _vertices.size();

        for (size_t i = 0; i <= _slices; ++i)
        {
          float _m   = 2.0 * M_PI * float(i) / _slices;
          float _cos = cos(_m), _sin = sin(_m);
          QVector3D _top(_cos * _topRadius, _sin * _topRadius, _zTop);
          QVector3D _bottom(_cos * _bottomRadius, _sin * _bottomRadius, _zBottom);
          QVector3D _normalTop(_top.normalized());
          QVector3D _normalBottom(_bottom.normalized());
          _vertices.emplace_back(_top, _normalTop,
                                 QPointF(float(i) / _slices,
                                         acos(_normalTop.z()) / M_PI));
          _vertices.emplace_back(_bottom, _normalBottom,
                                 QPointF(float(i) / _slices,
                                         acos(_normalBottom.z()) / M_PI));
        }

        for (size_t i = 0; i < _slices; ++i)
        {
          /// Top triangle
          indices.push_back(_startIndex + 2 * i);
          indices.push_back(_startIndex + 2 * i + 1);
          indices.push_back(_startIndex + 2 * (i + 1));

          /// Bottom triangle
          indices.push_back(_startIndex + 2 * i + 1);
          indices.push_back(_startIndex + 2 * (i + 1) + 1);
          indices.push_back(_startIndex + 2 * (i + 1));
        }
      }

      /// Generate a sphere
      template<typename VERTICES, typename INDICES>
      static void generateSphere(size_t _stacks,
                                 size_t _slices,
                                 VERTICES& _vertices,
                                 INDICES& indices)
      {
        // If there are M lines of latitude (horizontal) and
        // N lines of longitude (vertical), then put dots at
        // (x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi *
        // n/N), cos(Pi * m/M))
        auto stackRadius = [&_stacks](size_t index)
                           {
                             return sin(M_PI * float(index) / _stacks);
                           };
        auto stackPos = [&_stacks](size_t index)
                        {
                          return cos(M_PI * float(index) / _stacks);
                        };

        /// Top cone
        generateCone(_slices, 1.0, stackPos(1), stackRadius(1), _vertices,
                     indices);

        /// Bottom cone
        generateCone(_slices, -1.0, stackPos(_stacks - 1), stackRadius(
                       1), _vertices, indices);

        for (size_t i = 1; i < _stacks - 1; ++i)
        {
          generateSphereStack(_slices,
                              stackPos(i), stackPos(i + 1),
                              stackRadius(i), stackRadius(i + 1),
                              _vertices, indices);
        }
      }

      /// Generate bottom cut off sphere
      template<typename VERTICES, typename INDICES>
      static void generateSphereUpper(size_t _stacks,
                                      size_t _slices,
                                      float _bottom,
                                      VERTICES& _vertices,
                                      INDICES& indices)
      {
        /// Top cone
        auto stackRadius = [&_stacks](size_t index)
                           {
                             return sin(M_PI * float(index) / _stacks);
                           };
        auto stackPos = [&_stacks](size_t index)
                        {
                          return cos(M_PI * float(index) / _stacks);
                        };

        generateCone(_slices, 1.0, stackPos(1), stackRadius(1), _vertices,
                     indices);

        for (size_t i = 1; i < _stacks - 1; ++i)
        {
          float _stackPos = stackPos(i + 1);

          if (_stackPos < _bottom) continue;
          generateSphereStack(_slices,
                              stackPos(i), stackPos(i + 1),
                              stackRadius(i), stackRadius(i + 1),
                              _vertices, indices);
        }
      }
    }
  }
}

#endif /* OMNI_CANVAS_UTIL_HPP_ */

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

#include <omni/visual/Sphere.h>

#include <omni/visual/util.h>
#include <QDebug>

namespace omni
{
  namespace visual
  {
    Sphere::Sphere(qreal _radius) :
      radius_(_radius),
      scale_(1.0,1.0,1.0)
    {
      update();
    }

    Sphere::~Sphere()
    {
    }

    qreal Sphere::radius() const
    {
      return radius_;
    }

    void Sphere::setRadius(qreal _radius)
    {
      radius_=_radius;
    }

    QVector3D Sphere::scale() const
    {
      return scale_;
    }

    void Sphere::setScale(QVector3D const& _scale)
    {
      scale_ = _scale;
    }

    int Sphere::stacks() const
    {
      return stacks_;
    }

    void Sphere::setStacks(int _stacks)
    {
      stacks_ = _stacks;
      update();
    }

    int Sphere::slices() const
    {
      return slices_;
    }

    void Sphere::setSlices(int _slices)
    {
      slices_ = _slices;
      update();
    }

    float Sphere::top() const
    {
      return top_;
    }

    void Sphere::setTop(float _top)
    {
      top_=_top;
      update();
    }

    float Sphere::bottom() const
    {
      return bottom_;
    }

    void Sphere::setBottom(float _bottom)
    {
      bottom_=_bottom;
      update();
    }

    void Sphere::draw() const
    {
      glPushMatrix();
      with_current_context([this](QOpenGLFunctions& _)
      {
        // Scale offset
        glScalef(radius_ * scale_.x(),radius_ * scale_.y(),radius_ * scale_.z());

        vbo_.bind();
        vbo_.draw();
        vbo_.unbind();
      });
      glPopMatrix();
    }

    void Sphere::update()
    {
      vertices_.clear();
      indices_.clear();
      vertices_.reserve(2 * slices_ * stacks_);
      indices_.reserve(6 * slices_ * stacks_);

      // If there are M lines of latitude (horizontal) and
      // N lines of longitude (vertical), then put dots at
      // (x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
      auto stackRadius = [this](size_t index)
      {
        return sin(M_PI * float(index) / stacks_);
      };
      auto stackPos = [this](size_t index)
      {
        return cos(M_PI * float(index) / stacks_);
      };

      for (size_t i = 0; i < stacks_; ++i)
      {
        if (stackPos(i) > top_ || stackPos(i+1) < bottom_) continue;

        generateStack(stackPos(i),stackPos(i+1),
                      stackRadius(i),stackRadius(i+1));
      }

      vbo_.buffer(vertices_,indices_);
      vertices_.clear();
      indices_.clear();
    }

    void Sphere::generateStack(float _top, float _bottom,
                               float _topRadius, float _bottomRadius)
    {
      size_t _startIndex = vertices_.size();
      for (size_t i = 0; i <= slices_; ++i)
      {
        /// Generate vertices
        float _m = 2.0 * M_PI * float(i) / slices_;
        float _cos = cos(_m), _sin = sin(_m);
        QVector3D _topPoint(_cos * _topRadius,_sin * _topRadius,_top);
        QVector3D _bottomPoint(_cos * _bottomRadius,_sin * _bottomRadius,_bottom);
        QVector3D _normalTop(_topPoint.normalized());
        QVector3D _normalBottom(_bottomPoint.normalized());
        vertices_.emplace_back(_topPoint,_normalTop,_normalTop);//QVector2D(float(i)/slices_,1.0 - acos(_normalTop.z()) / M_PI));
        vertices_.emplace_back(_bottomPoint,_normalBottom,_normalBottom);//QVector2D(float(i)/slices_,1.0 - acos(_normalBottom.z()) / M_PI));

        /// Top triangle
        indices_.push_back(_startIndex + 2 * i);
        indices_.push_back(_startIndex + 2 * i + 1);
        indices_.push_back(_startIndex + 2 * (i + 1) );

        /// Bottom triangle
        indices_.push_back(_startIndex + 2 * i + 1);
        indices_.push_back(_startIndex + 2 * (i + 1) +1 );
        indices_.push_back(_startIndex + 2 * (i + 1) );
      }
    }
  }
}

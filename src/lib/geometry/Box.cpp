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

#include <omni/geometry/Box.h>

#include <limits>

namespace omni {
  namespace geometry {
    Box::Box()
    {
      auto _min = std::numeric_limits<float>::min();
      auto _max = std::numeric_limits<float>::max();

      min_ = QVector3D(_max, _max, _max);
      max_ = QVector3D(_min, _min, _min);
    }

    Box::Box(QVector3D const& _min, QVector3D const& _max)
    {
      setMinMax(_min, _max);
    }

    QVector3D Box::size() const
    {
      return max() - min();
    }

    qreal Box::radius() const
    {
      return size().length() * 0.5;
    }

    QVector3D Box::min() const
    {
      return min_;
    }

    QVector3D Box::max() const
    {
      return max_;
    }

    void Box::setMin(QVector3D const& _min)
    {
      min_ = _min;
      validate();
    }

    void Box::setMax(QVector3D const& _max)
    {
      max_ = _max;
      validate();
    }

    void Box::setMinMax(QVector3D const& _min, QVector3D const& _max)
    {
      min_ = _min;
      max_ = _max;
      validate();
    }

    QVector3D Box::center() const {
      return 0.5 * (min_ + max_);
    }

    void Box::extend(QVector3D const& _v) {
      for (int i = 0; i < 3; ++i) {
        min_[i] = std::min(_v[i], min_[i]);
        max_[i] = std::max(_v[i], max_[i]);
      }
      validate();
    }

    void Box::validate()
    {
      for (int i = 0; i < 3; ++i)
      {
        if (min_[i] > max_[i]) std::swap(min_[i], max_[i]);
      }
    }
  }
}

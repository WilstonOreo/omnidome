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

#include <omni/visual/Tracker.h>

#include <algorithm>
#include <omni/serialization/PropertyMap.h>

namespace omni {
  namespace visual {
    Tracker::Tracker() :
      center_(0.0, 0.0, 0.0),
      direction_(0.0, 0.0, 1.0)
    {}

    Tracker::Tracker(
      const QVector3D& _center,
      const PolarVec& _direction) :
      center_(_center),
      direction_(_direction)
    {}

    void Tracker::track(float _longitude, float _latitude, float _radius)
    {
      direction_ += PolarVec(Angle(_longitude), Angle(_latitude), _radius);
    }

    QVector3D Tracker::eye() const
    {
      return center_ + direction_.vec();
    }

    void Tracker::setEye(const QVector3D& _pos)
    {
      direction_ = _pos - center_;
    }

    void Tracker::setCenter(QVector3D const& _center)
    {
      center_ = _center;
    }

    QVector3D& Tracker::center()
    {
      return center_;
    }

    QVector3D const& Tracker::center() const
    {
      return center_;
    }

    void Tracker::setDirection(PolarVec const& _direction)
    {
      direction_ = _direction;
    }

    void Tracker::setDistance(float _t)
    {
      direction_ = _t * direction_.normalized();
    }

    PolarVec& Tracker::direction()
    {
      return direction_;
    }

    PolarVec const& Tracker::direction() const
    {
      return direction_;
    }

    void Tracker::limitDistance(float _minDist, float _maxDist)
    {
      float _r = direction_.radius();

      if (_minDist > _maxDist) std::swap(_minDist, _maxDist);

      if (_r < _minDist) direction_.setRadius(_minDist);

      if (_r > _maxDist) direction_.setRadius(_maxDist);
    }

    /// Deserialize from stream
    void Tracker::fromStream(QDataStream& _is)
    {
      PropertyMap _map;
      _is >> _map;
      _map.get("center",center_);
      _map.get("direction",direction_);
    }

    /// Serialize to stream
    void Tracker::toStream(QDataStream& _os) const
    {
      PropertyMap _map;
      _map("center",center_);
      _map("direction",direction_);
      _os << _map;
    }

    /// Test for equality. ScreenSetup is ignored
    bool  operator==(Tracker const& _lhs,
                     Tracker const& _rhs)
    {
      return
        OMNI_TEST_MEMBER_EQUAL(center_) &&
        OMNI_TEST_MEMBER_EQUAL(direction_);
    }
  }
}

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

#include <omni/visual/CameraInterface.h>

#include <QMatrix4x4>

namespace omni {
  namespace visual {
    CameraInterface::CameraInterface() :
      up_(0.0, 0.0, 1.0)
    {}

    CameraInterface::CameraInterface(const Tracker& _tracker,
                   qreal _near,
                   qreal _far,
                   const QVector3D& _up) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {}

    QVector3D CameraInterface::right() const
    {
      return QVector3D::crossProduct(Tracker::direction().vec(), up_);
    }

    void CameraInterface::strafe(qreal _right)
    {
      Tracker::setCenter(Tracker::center() + right().normalized() * _right);
    }

    void CameraInterface::lift(qreal _up)
    {
      QVector3D _orthoUp = QVector3D::crossProduct(right(),
                                                   Tracker::direction().vec());
      Tracker::setCenter(Tracker::center() + _orthoUp.normalized() * _up);
    }

    QVector3D CameraInterface::up() const
    {
      return up_;
    }

    void CameraInterface::setUp(QVector3D const& _up)
    {
      up_ = _up.normalized();
    }

    qreal CameraInterface::near() const
    {
      return near_;
    }

    void CameraInterface::setNear(qreal _near)
    {
      near_ = _near;
    }

    qreal CameraInterface::far() const
    {
      return far_;
    }

    void CameraInterface::setFar(qreal _far)
    {
      far_ = _far;
    }

    /// Deserialize from stream
    void CameraInterface::fromStream(QDataStream& _is) {
      Tracker::fromStream(_is);
      _is >> up_ >> near_ >> far_;
    }

    /// Serialize to stream
    void CameraInterface::toStream(QDataStream& _os) const {
      Tracker::toStream(_os);
      _os << up_ << near_ << far_;
    }

    /// Test for equality.
    bool operator==(CameraInterface const& _lhs, CameraInterface const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(up_) &&
        OMNI_TEST_MEMBER_EQUAL(near_) &&
        OMNI_TEST_MEMBER_EQUAL(far_);
    }
  }
}

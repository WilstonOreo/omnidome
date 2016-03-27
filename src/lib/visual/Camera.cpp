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

#include <omni/visual/Camera.h>

#include <QMatrix4x4>

namespace omni {
  namespace visual {
    Camera::Camera() :
      up_(0.0, 0.0, 1.0)
    {}

    Camera::Camera(const Tracker& _tracker,
                   qreal _near,
                   qreal _far,
                   qreal _fov,
                   const QVector3D& _up) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {}

    QVector3D Camera::right() const
    {
      return QVector3D::crossProduct(Tracker::direction().vec(), up_);
    }

    void Camera::strafe(qreal _right)
    {
      Tracker::setCenter(Tracker::center() + right().normalized() * _right);
    }

    void Camera::lift(qreal _up)
    {
      QVector3D _orthoUp = QVector3D::crossProduct(right(),
                                                   Tracker::direction().vec());
      Tracker::setCenter(Tracker::center() + _orthoUp.normalized() * _up);
    }

    QVector3D Camera::up() const
    {
      return up_;
    }

    void Camera::setUp(QVector3D const& _up)
    {
      up_ = _up.normalized();
    }

    qreal Camera::near() const
    {
      return near_;
    }

    void Camera::setNear(qreal _near)
    {
      near_ = _near;
    }

    qreal Camera::far() const
    {
      return far_;
    }

    void Camera::setFar(qreal _far)
    {
      far_ = _far;
    }

    qreal Camera::fov() const {
      return fov_;
    }

    void Camera::setFov(qreal _fov) {
      fov_ = _fov;
    }

    /// Setup perspective camera in OpenGL
    void Camera::setup(qreal _fov, qreal _aspect)
    {
      QMatrix4x4 _m;

      fov_ = _fov;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      // perspective projection
      _m.perspective(fov_, _aspect, near(), far());

      _m.lookAt(eye(), center(), up());

      // Apply matrix to OpenGL
      glMultMatrixf(_m.constData());
    }

    /// Deserialize from stream
    void Camera::fromStream(QDataStream& _is) {
      Tracker::fromStream(_is);
      _is >> up_ >> near_ >> far_ >> fov_;
    }

    /// Serialize to stream
    void Camera::toStream(QDataStream& _os) const {
      Tracker::toStream(_os);
      _os << up_ << near_ << far_ << fov_;
    }

    /// Test for equality. ScreenSetup is ignored
    bool operator==(Camera const& _lhs, Camera const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(up_) &&
        OMNI_TEST_MEMBER_EQUAL(near_) &&
        OMNI_TEST_MEMBER_EQUAL(far_) &&
        OMNI_TEST_MEMBER_EQUAL(fov_);
    }
  }
}

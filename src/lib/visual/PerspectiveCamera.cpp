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

#include <omni/visual/PerspectiveCamera.h>
#include <QMatrix4x4>

namespace omni {
  namespace visual {
    PerspectiveCamera::PerspectiveCamera(qreal _fov) :
      fov_(_fov) {
    }

    PerspectiveCamera::PerspectiveCamera(const Tracker& _tracker,
                   qreal _fov,
                   qreal _near,
                   qreal _far,
                   const QVector3D& _up) :
      CameraInterface(_tracker,_near,_far,_up),
      fov_(_fov)
    {

    }

    qreal PerspectiveCamera::fov() const {
      return fov_;
    }

    void PerspectiveCamera::setFov(qreal _fov) {
      fov_ = _fov;
    }

    void PerspectiveCamera::setup(qreal _aspect)
    {
      matrix_ = QMatrix4x4();

      // perspective projection
      matrix_.perspective(fov_, _aspect, near(), far());
      matrix_.lookAt(eye(), center(), up());
    }

    /// Deserialize from stream
    void PerspectiveCamera::fromStream(QDataStream& _is) {
      CameraInterface::fromStream(_is);
      _is >> fov_;
    }

    /// Serialize to stream
    void PerspectiveCamera::toStream(QDataStream& _os) const {
      CameraInterface::toStream(_os);
      _os << fov_;
    }

    /// Test for equality. ScreenSetup is ignored
    bool operator==(PerspectiveCamera const& _lhs, PerspectiveCamera const& _rhs) {
      return
        (static_cast<CameraInterface const&>(_lhs) == static_cast<CameraInterface const&>(_rhs)) &&
        OMNI_TEST_MEMBER_EQUAL(fov_);
    }
  }
}

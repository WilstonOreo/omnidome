/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#include <omni/proj/Projector.h>

#include <omni/proj/ScreenSetup.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/serialization/pointer.h>
#include <omni/proj/Frustum.h>

namespace omni {
  namespace proj {
    Projector::Projector() :
      fov_(45.0)
    {}

    Projector::Projector(qreal _aspectRatio,
                         Angle _fov) :
      aspectRatio_(_aspectRatio),
      fov_(_fov)
    {}

    Setup * Projector::setup(Id const& _setupId, float _scale)
    {
      setup_.reset(SetupFactory::create(_setupId));
      setup()->scale(_scale);
      return setup();
    }

    Setup * Projector::setup()
    {
      if (!setup_) return nullptr;

      setup_->setup(*this);
      return setup_.get();
    }

    Setup const * Projector::setup() const
    {
      return setup_.get();
    }

    qreal Projector::aspectRatio() const
    {
      return aspectRatio_;
    }

    void Projector::setAspectRatio(qreal _aspectRatio) {
      aspectRatio_ = _aspectRatio;
    }

    qreal Projector::throwRatio() const
    {
      return 0.5 / tan(fov_.radians() / 2.0);
    }

    void Projector::setThrowRatio(qreal _throwRatio)
    {
      fov_ = Angle::fromRad(2.0 * atan(1.0 / (_throwRatio * 2.0)));
    }

    void Projector::setFov(Angle _fov)
    {
      fov_ = _fov;

      if (fov_.degrees() < 1.0) fov_ = 1.0;

      if (fov_.degrees() >= 180.0) fov_ = 180.0;
    }

    Angle Projector::fov() const
    {
      return fov_;
    }

    qreal Projector::keystone() const {
      return keystone_;
    }

    void Projector::setKeystone(qreal _keystone) {
      keystone_ = _keystone;
    }

    QMatrix4x4 const& Projector::matrix() const
    {
      return matrix_;
    }

    void Projector::setMatrix(QMatrix4x4 const& _matrix)
    {
      matrix_ = _matrix;
    }

    QMatrix4x4 Projector::projectionMatrix() const
    {
      QMatrix4x4 _m;

      Frustum _frustum(*this);
      float   _near = 0.01;
      QRectF  _rect = _frustum.rect(_near, 1000.0);

      _m.frustum(_rect.left(), _rect.right(), _rect.bottom(),
                 _rect.top(), _near, 1000.0);
      _m.lookAt(pos(), pos() + matrix().column(0).toVector3D(), matrix().column(
                  2).toVector3D());
      return _m;
    }

    QVector3D Projector::pos() const
    {
      return matrix().column(3).toVector3D();
    }

    void Projector::toStream(QDataStream& _os) const {
      PropertyMap _map;

      _map("fov", fov_);
      _map("keystone", keystone_);
      _map("aspectRatio", aspectRatio_);
      _map("setup", setup_);
      _os << _map;
    }

    void Projector::fromStream(QDataStream& _is) {
      PropertyMap _map;
      _is >> _map;
      _map.get("fov", fov_);
      _map.get("keystone", keystone_);
      _map.get("aspectRatio",aspectRatio_);
      _map.getPtr("setup", [this](const Id& _id) -> Setup * {
        return this->setup(_id);
      });
    }

    bool operator==(Projector const& _lhs, Projector const& _rhs)
    {
      return
        OMNI_TEST_MEMBER_EQUAL(matrix_) &&
        OMNI_TEST_MEMBER_EQUAL(fov_) &&
        OMNI_TEST_MEMBER_EQUAL(keystone_) &&
        OMNI_TEST_MEMBER_EQUAL(aspectRatio_) &&
        OMNI_TEST_PTR_MEMBER_EQUAL(setup_);
    }
  }
}

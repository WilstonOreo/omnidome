/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#include <cmath>
#include <QVector2D>
#include <omni/proj/ScreenSetup.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/serialization/pointer.h>
#include <omni/proj/Frustum.h>

namespace omni {
  namespace proj {
    Projector::Projector(Setup _setup) :
      fov_(45.0)
    {
      setSetup(_setup);
    }

    Projector::Setup Projector::setup() const
    {
      return setup_;
    }

    void Projector::setSetup(Setup _setup) {
      setup_ = _setup;
      update();
    }


    void Projector::setupFree(EulerAngles const& _angles, QVector3D const& _pos) {
        static_cast<EulerAngles&>(*this) = _angles;
        pos_ = _pos;
        setSetup(FREE);
    }

    void Projector::setupPeripheral(EulerAngles const& _angles,
                            Angle _deltaYaw,
                            qreal _distanceCenter,
                            qreal _towerHeight,
                            qreal _shift
                          ) {
        static_cast<EulerAngles&>(*this) = _angles;
        deltaYaw_ = _deltaYaw;
        distanceCenter_ = _distanceCenter;
        towerHeight_ = _towerHeight;
        shift_ = _shift;
        setSetup(PERIPHERAL);
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

    QVector3D const& Projector::pos() const
    {
      return pos_;
    }

    void Projector::setPos(QVector3D const& _pos)
    {
      pos_ = _pos;
      update();
    }

    void Projector::setPos(qreal _x, qreal _y, qreal _z)
    {
      setPos(QVector3D(_x,_y,_z));
    }


    Angle Projector::deltaYaw() const
    {
      return deltaYaw_;
    }

    void Projector::setDeltaYaw(Angle _deltaYaw)
    {
      deltaYaw_=_deltaYaw;
      update();
    }

    qreal Projector::distanceCenter() const
    {
      return distanceCenter_;
    }

    void Projector::setDistanceCenter(qreal _distanceCenter)
    {
      distanceCenter_=_distanceCenter;
      update();
    }

    qreal Projector::towerHeight() const
    {
      return towerHeight_;
    }

    void Projector::setTowerHeight(qreal _towerHeight)
    {
      towerHeight_ = _towerHeight;
      update();
    }

    qreal Projector::shift() const
    {
      return shift_;
    }

    void Projector::setShift(qreal _shift)
    {
      shift_=_shift;
      update();
    }

    void Projector::scale(qreal _scale) {
      pos_ *= _scale;
      distanceCenter_ *= _scale;
      shift_ *= _scale;
      towerHeight_ *= _scale;
    }

    bool Projector::flipped() const {
      return std::abs(roll().degrees()) > 90.0;
    }

    void Projector::toStream(QDataStream& _os) const {
      PropertyMap _map;
      _map("setup",setup_);
      _map("angles",static_cast<EulerAngles const&>(*this));
      _map("pos",pos_);
      _map("fov", fov_);
      _map("keystone", keystone_);
      _map("aspectRatio", aspectRatio_);
      _map("deltaYaw",deltaYaw_);
      _map("distanceCenter",distanceCenter_);
      _map("towerHeight",towerHeight_);
      _map("shift",shift_);
      _os << _map;
    }

    void Projector::fromStream(QDataStream& _is) {
      PropertyMap _map;
      _is >> _map;
      _map.get("setup",setup_);
      _map.get("angles",static_cast<EulerAngles&>(*this));
      _map.get("pos",pos_);
      _map.get("fov", fov_);
      _map.get("keystone", keystone_);
      _map.get("aspectRatio",aspectRatio_);
      _map.get("deltaYaw",deltaYaw_);
      _map.get("distanceCenter",distanceCenter_);
      _map.get("towerHeight",towerHeight_);
      _map.get("shift",shift_);

      update();
    }

    bool operator==(Projector const& _lhs, Projector const& _rhs)
    {
      return
        (static_cast<EulerAngles const&>(_lhs) == static_cast<EulerAngles const&>(_rhs)) &&
        OMNI_TEST_MEMBER_EQUAL(matrix_) &&
        OMNI_TEST_MEMBER_EQUAL(fov_) &&
        OMNI_TEST_MEMBER_EQUAL(keystone_) &&
        OMNI_TEST_MEMBER_EQUAL(aspectRatio_) &&
        OMNI_TEST_MEMBER_EQUAL(pos_) &&
        OMNI_TEST_MEMBER_EQUAL(deltaYaw_) &&
        OMNI_TEST_MEMBER_EQUAL(distanceCenter_) &&
        OMNI_TEST_MEMBER_EQUAL(towerHeight_) &&
        OMNI_TEST_MEMBER_EQUAL(shift_);
    }

    void Projector::update() {
      QMatrix4x4 _m;

      switch (setup_) {
        case FREE:
        _m.translate(pos_);
        _m *= EulerAngles::matrix();
        break;
        case PERIPHERAL: {
        qreal _theta = yaw().radians();
        qreal _ct = -cos(_theta), _st = -sin(_theta);
        QVector2D _shiftVec = QVector2D(-_st,_ct) * shift_;
        QVector2D _p = _shiftVec +  distanceCenter_ * QVector2D(_ct,_st);
        QVector3D _pos = QVector3D(_p.x(),_p.y(),towerHeight_);
        pos_ = _pos;
        _m.translate(_pos);
        _m.rotate(yaw().degrees() + deltaYaw_.degrees(),QVector3D(0.0,0.0,1.0));
        _m.rotate(-pitch().degrees(),QVector3D(0.0,1.0,0.0));
        _m.rotate(roll().degrees(),QVector3D(1.0,0.0,0.0));
        }
        break;
      }

      setMatrix(_m);
    }
  }
}

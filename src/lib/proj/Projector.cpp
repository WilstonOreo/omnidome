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

#include <QVector2D>
#include <omni/proj/ScreenSetup.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/serialization/pointer.h>
#include <omni/proj/Frustum.h>

namespace omni {
  namespace proj {
    Projector::Projector(QObject* parent) :
      QObject(parent),
      angles_(new EulerAngles(this)),
      fov_(45.0)
    {
      connect(this,&Projector::setupChanged,this,&Projector::update);
      connect(this,&Projector::deltaYawChanged,this,&Projector::update);
      connect(this,&Projector::posChanged,this,&Projector::update);
      connect(this,&Projector::keystoneChanged,this,&Projector::update);
      connect(this,&Projector::aspectRatioChanged,this,&Projector::update);
      connect(this,&Projector::distanceCenterChanged,this,&Projector::update);
      connect(this,&Projector::towerHeightChanged,this,&Projector::update);
      connect(this,&Projector::shiftChanged,this,&Projector::update);
      update();
    }

    EulerAngles*  Projector::angles() const {
      return angles_;
    }

    qreal Projector::throwRatio() const
    {
      return 0.5 / tan(fov_.radians() / 2.0);
    }

    void Projector::setThrowRatio(qreal _throwRatio)
    {
      setFov(Angle::fromRad(2.0 * atan(1.0 / (_throwRatio * 2.0))));
    }

    Angle Projector::fov() const
    {
      return fov_;
    }

    void Projector::setFov(Angle _fov)
    {
      if (fov_ != _fov) {
        fov_ = _fov;
        if (fov_.degrees() < 1.0) fov_ = 1.0;
        if (fov_.degrees() >= 180.0) fov_ = 180.0;
        emit fovChanged();
      }
    }

    QMatrix4x4 const& Projector::matrix() const
    {
      return matrix_;
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


    void Projector::setPos(qreal _x, qreal _y, qreal _z)
    {
      setPos(QVector3D(_x,_y,_z));
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

    void Projector::update() {
      QMatrix4x4 _m;

      switch (setup_) {
        case FREE:
        _m.translate(pos_);
        _m *= angles()->matrix();
        break;
        case PERIPHERAL: {
        qreal _theta = angles()->yaw().radians();
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
      matrix_ = _m;
      emit changed();
    }

    OMNI_PROPERTY_RW_IMPL(Projector,Angle,deltaYaw,setDeltaYaw)
    OMNI_PROPERTY_RW_IMPL(Projector,qreal,aspectRatio,setAspectRatio)
    OMNI_PROPERTY_RW_IMPL(Projector,qreal,distanceCenter,setDistanceCenter)
    OMNI_PROPERTY_RW_IMPL(Projector,qreal,towerHeight,setTowerHeight)
    OMNI_PROPERTY_RW_IMPL(Projector,qreal,shift,setShift)
    OMNI_PROPERTY_RW_IMPL(Projector,Projector::Setup,setup,setSetup)
    OMNI_PROPERTY_RW_IMPL(Projector,QVector3D,pos,setPos)
    OMNI_PROPERTY_RW_IMPL(Projector,qreal,keystone,setKeystone)
  }
}

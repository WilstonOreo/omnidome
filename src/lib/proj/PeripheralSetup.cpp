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

#include <omni/proj/PeripheralSetup.h>

#include <QMatrix4x4>
#include <QVector2D>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    PeripheralSetup::PeripheralSetup() :
      deltaYaw_(0.0),
      distanceCenter_(4.0),
      towerHeight_(2.0),
      shift_(0.0)
    {
    }

    void PeripheralSetup::toStream(QDataStream& _stream) const
    {
      AngleSetup::toStream(_stream);
      _stream << deltaYaw_ << distanceCenter_ << towerHeight_ << shift_;
    }

    void PeripheralSetup::fromStream(QDataStream& _stream)
    {
      AngleSetup::fromStream(_stream);
      _stream >> deltaYaw_ >> distanceCenter_ >> towerHeight_ >> shift_;
    }

    void PeripheralSetup::setup(Projector& _proj)
    {
      QMatrix4x4 _matrix;

      qreal _theta = yaw().radians();
      qreal _ct = -cos(_theta), _st = -sin(_theta);
      QVector2D _shiftVec = QVector2D(-_st,_ct) * shift_;
      QVector2D _p = _shiftVec +  distanceCenter_ * QVector2D(_ct,_st);
      QVector3D _pos = QVector3D(_p.x(),_p.y(),towerHeight_);
      _matrix.translate(_pos);

      _matrix.rotate(yaw().degrees() + deltaYaw_.degrees(),QVector3D(0.0,0.0,1.0));
      _matrix.rotate(-pitch().degrees(),QVector3D(0.0,1.0,0.0));
      _matrix.rotate(roll().degrees(),QVector3D(1.0,0.0,0.0));
      _proj.setMatrix(_matrix);
    }

    Angle PeripheralSetup::deltaYaw() const
    {
      return deltaYaw_;
    }

    void PeripheralSetup::setDeltaYaw(Angle _deltaYaw)
    {
      deltaYaw_=_deltaYaw;
    }

    qreal PeripheralSetup::distanceCenter() const
    {
      return distanceCenter_;
    }

    void PeripheralSetup::setDistanceCenter(qreal _distanceCenter)
    {
      distanceCenter_=_distanceCenter;
    }

    qreal PeripheralSetup::towerHeight() const
    {
      return towerHeight_;
    }

    void PeripheralSetup::setTowerHeight(qreal _towerHeight)
    {
      towerHeight_ = _towerHeight;
    }

    qreal PeripheralSetup::shift() const
    {
      return shift_;
    }

    void PeripheralSetup::setShift(qreal _shift)
    {
      shift_=_shift;
    }
  }
}

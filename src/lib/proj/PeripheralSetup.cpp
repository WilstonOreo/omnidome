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
      distanceCenter_(5.0),
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

      _matrix.rotate(yaw().degrees() + deltaYaw_.degrees(),QVector3D(0.0,0.0,1.0));
      _matrix.rotate(-pitch().degrees(),QVector3D(0.0,1.0,0.0));
      _matrix.rotate(roll().degrees(),QVector3D(1.0,0.0,0.0));
      _matrix.translate(_pos);
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

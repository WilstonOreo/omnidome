#include <omni/PolarVec.h>

namespace omni
{
  PolarVec::PolarVec()
  {
  }

  PolarVec::PolarVec(const QVector3D& _vec)
  {
    operator=(_vec);
  }

  PolarVec::PolarVec(const QVector4D& _vec)
  {
    if (_vec.w() != 0.0)
      operator=(QVector3D(_vec.x()/_vec.w(),_vec.y()/_vec.w(),_vec.z()/_vec.w()));
    else
      operator=(QVector3D(_vec.x(),_vec.y(),_vec.z()));
  }

  PolarVec::PolarVec(Angle _longitude, Angle _latitude, qreal _radius) :
    longitude_(_longitude),
    latitude_(_latitude),
    radius_(_radius)
  {
  }

  const PolarVec& PolarVec::operator=(const QVector3D& _vec)
  {
    radius_ = _vec.length();
    longitude_ = Angle::fromRad(atan2(_vec.y(), _vec.x()));
    latitude_ = (radius_ != 0.0) ? Angle::fromRad(acos(_vec.z() / radius_)) : 0.0;
    return *this;
  }

  PolarVec::operator QVector3D() const
  {
    qreal _phi = longitude_.radians();
    qreal _theta = latitude_.radians();
    return radius() * QVector3D(
             sin(_theta) * cos(_phi),
             sin(_theta) * sin(_phi),
             cos(_theta));
  }

  PolarVec::operator QVector4D() const
  {
    QVector3D _vec = *this;
    return QVector4D(_vec.x(),_vec.y(),_vec.z(),1.0);
  }

  const PolarVec& PolarVec::operator+=( const PolarVec& _vec )
  {
    // add longitude
    longitude_ += _vec.longitude_;
    // add latitude
    latitude_ = Angle( std::min<qreal>(179.99,std::max<qreal>(0.01,latitude_.degrees() + _vec.latitude_.degrees())));
    // add radius
    radius_ = std::max<qreal>(0.0,radius_ + _vec.radius_);
    return *this;
  }
  
  qreal PolarVec::x() const
  {
    return static_cast<QVector3D const&>(*this).x();
  }

  qreal PolarVec::y() const
  {
    return static_cast<QVector3D const&>(*this).y();
  }

  qreal PolarVec::z() const
  {
    return static_cast<QVector3D const&>(*this).z();
  }
    
  QVector3D PolarVec::vec() const
  {
    return static_cast<QVector3D const&>(*this);
  }

  QVector3D PolarVec::normalized() const
  {
    return static_cast<QVector3D const&>(*this).normalized();
  }

  Angle PolarVec::longitude() const
  {
    return longitude_;
  }

  void PolarVec::setLongitude(Angle _longitude)
  {
    longitude_ = _longitude;
  }

  Angle PolarVec::latitude() const
  {
    return latitude_;
  }

  void PolarVec::setLatitude(Angle _latitude)
  {
    latitude_ = _latitude;
  }

  qreal PolarVec::radius() const
  {
    return radius_;
  }

  void PolarVec::setRadius(qreal _radius)
  {
    radius_=_radius;
  }
}

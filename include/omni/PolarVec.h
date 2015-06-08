#ifndef OMNI_POLARVEC_H_
#define OMNI_POLARVEC_H_

#include <QVector3D>
#include <QVector4D>
#include <omni/Angle.h>

namespace omni
{
  /** @brief polar 3D vector defined by th two angles phi, theta and a radial distance
   **/
  struct PolarVec
  {
    /// default constructor
    PolarVec();
    PolarVec(const QVector3D&);
    PolarVec(const QVector4D&);
    PolarVec(Angle _longitude, Angle _latitude, qreal _radius);

    const PolarVec& operator=( const QVector3D& );

    operator QVector3D() const;
    operator QVector4D() const;

    const PolarVec& operator+=( const PolarVec& );

    qreal x() const;
    qreal y() const;
    qreal z() const;
    QVector3D vec() const;
    QVector3D normalized() const;

    Angle longitude() const;
    void setLongitude(Angle);

    Angle latitude() const;
    void setLatitude(Angle);

    qreal radius() const;
    void setRadius(qreal);

  private:
    Angle longitude_, latitude_;
    qreal radius_;
  };
}


#endif /* OMNI_POLARVEC_H_ */


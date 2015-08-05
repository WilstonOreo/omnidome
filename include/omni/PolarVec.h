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

    /// Construct from QVector3D 
    PolarVec(const QVector3D&);

    /// Construct from QVector4D
    PolarVec(const QVector4D&);
    
    /// Construct from longitude angle, latitude angle and radius
    PolarVec(Angle _longitude, Angle _latitude, qreal _radius);

    /// Assign from QVector3D
    const PolarVec& operator=( const QVector3D& );

    /// QVector3D cast operator
    operator QVector3D() const;
    
    /// QVector4D cast operator
    operator QVector4D() const;

    /// Increase by other PolarVec
    const PolarVec& operator+=( const PolarVec& );

    /// Return x value (by converting to QVector3D internally)
    qreal x() const;
    
    /// Return y value (by converting to QVector3D internally)
    qreal y() const;
    
    /// Return z value (by converting to QVector3D internally)
    qreal z() const;
     
    /// Return converted QVector3D 
    QVector3D vec() const;

    /// Returns normalized QVector3D
    QVector3D normalized() const;

    /// Return longitudinal angle
    Angle longitude() const;

    /// Set longitudinal angle
    void setLongitude(Angle);

    /// Return latitudinal angle
    Angle latitude() const;
    
    /// Set latitudinal angle
    void setLatitude(Angle);

    /// Return radius
    qreal radius() const;
    
    /// Set radius
    void setRadius(qreal);

  private:
    Angle longitude_, latitude_;
    qreal radius_;
  };
}


#endif /* OMNI_POLARVEC_H_ */


#ifndef OMNI_VISUAL_TRACKER_H_
#define OMNI_VISUAL_TRACKER_H_ 

#include <omni/PolarVec.h>

namespace omni
{
  namespace visual
  {
    struct Tracker
    {
      Tracker();
      Tracker(const QVector3D& _center, const PolarVec& _direction);
      
      void track( float _longitude, float _latitude, float _radius );
      
      /// get tracker position
      QVector3D eye() const;

      void setEye( const QVector3D& _pos );

      void setCenter(QVector3D const&);
      QVector3D& center();
      QVector3D const& center() const;
      
      void setDirection(PolarVec const&);
      
      /// Set direction with a certain distance
      void setDistance(float _t); 

      PolarVec& direction();
      PolarVec const& direction() const;

    private:
      /// target position
      QVector3D center_;
      
      /// tracker's postion relatively to target
      PolarVec direction_; 
    };
  }
}

#endif /* OMNI_VISUAL_TRACKER_H_ */

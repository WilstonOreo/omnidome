#ifndef OMNI_EULERANGLES_H_
#define OMNI_EULERANGLES_H_

#include <QMatrix4x4>
#include <omni/Angle.h>

namespace omni
{
  /// Mixin class for a zyx Euler Angle Rotation system
  struct EulerAngles
  {
    /// Default constructor, all angles are zero
    EulerAngles(); 

    /// Constructor by values
    EulerAngles(Angle _yaw, Angle _pitch, Angle _roll);

    /// Reference to yaw angle (Angle in Z direction)
    Angle& yaw();
    
    /// Reference to yaw angle (Angle in Z direction, const version)
    Angle const& yaw() const;
    
    /// Set yaw angle (Z direction angle) to new value
    void setYaw(Angle _yaw);

    /// Reference to pitch angle (Angle in Y direction)
    Angle& pitch();
    
    /// Reference to pitch angle (Angle in Y direction, const version)
    Angle const& pitch() const;

    /// Set pitch angle (Y direction angle) to new value
    void setPitch(Angle _pitch);

    /// Reference to roll angle (Angle in X direction)
    Angle& roll();
    
    /// Reference to roll angle (Angle in X direction)
    Angle const& roll() const;
    
    /// Set roll angle (X direction angle) to new value
    void setRoll(Angle _roll);

    /// Public Static function to get rotation matrix conveniently 
    static QMatrix4x4 matrix(Angle _yaw, Angle _pitch, Angle _roll);
    
    /// Calculate the rotation matrix
    QMatrix4x4 matrix() const;

  private:
    /// Yaw Angle (Z direction)
    Angle yaw_; 

    /// Pitch Angle (Y direction)
    Angle pitch_; 

    /// Roll Angle (X direction)
    Angle roll_;
  };
}


QDataStream& operator>>(QDataStream&, omni::EulerAngles&);
QDataStream& operator<<(QDataStream&, omni::EulerAngles const&);



#endif /* OMNI_EULERANGLES_H_ */


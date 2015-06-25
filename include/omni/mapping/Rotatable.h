#ifndef OMNI_MAPPING_ROTATABLE_H_
#define OMNI_MAPPING_ROTATABLE_H_

#include <omni/Angle.h>
#include <omni/mapping/Interface.h>

namespace omni
{
  namespace mapping
  {
    /// A mapping which is rotatable with euler angles roll, pitch, yaw
    class Rotatable : public Interface
    {
    public:
      Rotatable(input::List const&);
      virtual ~Rotatable();

      /// Bind shader and set uniforms yaw_angle, pitch_angle and roll_angle
      virtual void bindShader();

      /// Return yaw angle (rotates around Z axis)
      Angle yaw() const;

      /// Set yaw angle (rotates around Z axis)
      void setYaw(Angle const&);
      
      /// Return pitch angle (rotates around Y axis)
      Angle pitch() const;
      
      /// Set pitch angle (rotates around Y axis)
      void setPitch(Angle const&);

      /// Set roll angle (rotates around X axis)
      Angle roll() const;
      
      /// Set roll angle (rotates around X axis)
      void setRoll(Angle const&);

      /// Deserialize from stream
      virtual void fromStream(QDataStream&);
      
      /// Serialize to stream 
      virtual void toStream(QDataStream&) const;
      
      /// Test for equality
      friend bool operator==(Rotatable const&,Rotatable const&);

    private:
      Angle yaw_, pitch_, roll_;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::mapping::Rotatable)

#endif /* OMNI_MAPPING_ROTATABLE_H_ */

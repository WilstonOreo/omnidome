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

      Angle yaw() const;
      void setYaw(Angle const&);
      
      Angle pitch() const;
      void setPitch(Angle const&);

      Angle roll() const;
      void setRoll(Angle const&);

      virtual void fromStream(QDataStream&);
      virtual void toStream(QDataStream&) const;

    private:
      Angle yaw_, pitch_, roll_;
    };
  }
}

#endif /* OMNI_MAPPING_ROTATABLE_H_ */

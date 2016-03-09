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
      Rotatable();
      virtual ~Rotatable();

      /// Bind shader and set uniforms yaw_angle, pitch_angle and roll_angle
      virtual void bind();

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

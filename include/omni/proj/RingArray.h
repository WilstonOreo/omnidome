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

#ifndef OMNI_PROJ_RINGARRAY_H_
#define OMNI_PROJ_RINGARRAY_H_

#include <omni/proj/MultiSetup.h>
#include <omni/Angle.h>

namespace omni
{
  namespace proj
  {
    class RingArray : public MultiSetup
    {
    public:
      OMNI_REGISTER_CLASS(MultiSetupFactory,RingArray)

      RingArray();
      ~RingArray();

      int numberOfProjectors() const;
      void setNumberOfProjectors(int);

      float distanceCenter() const;
      void setDistanceCenter(float);

      float towerHeight() const;
      void setTowerHeight(float);

      Angle pitch() const;
      void setPitch(Angle const&);

      Angle yaw() const;
      void setYaw(Angle const&);

      Angle fov() const;
      void setFov(Angle const&);

      std::vector<Projector> projectors() const;


    private:
      int numberOfProjectors_ = 4;
      float distanceCenter_ = 4.0;
      float towerHeight_ = 0.0;
      Angle pitch_, yaw_;
      Angle fov_;
    };
  }
}

#endif /* OMNI_PROJ_RINGARRAY_H_ */

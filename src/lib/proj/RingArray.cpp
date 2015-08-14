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

#include <omni/proj/RingArray.h>

#include <omni/proj/PeripheralSetup.h>

namespace omni
{
  namespace proj
  {
    RingArray::RingArray() :
      pitch_(30),
      yaw_(0)
    {
      setFov(55.0);
    }

    RingArray::~RingArray()
    {
    }
      
    std::vector<Projector> RingArray::projectors() const
    {
      std::vector<Projector> _projectors;

      for (int i = 0; i < numberOfProjectors(); ++i)
      {
        Projector _proj;
        _proj.setFov(fov_);

        auto* _setup = _proj.setup("PeripheralSetup");
        if (!_setup) continue;

        auto _peripheral = static_cast<PeripheralSetup*>(_setup);
 
        _peripheral->setPitch(pitch_);
        _peripheral->setYaw(Angle(i * 360.0 / numberOfProjectors() + yaw_.degrees()));
        _peripheral->setDistanceCenter(distanceCenter_);
        _peripheral->setTowerHeight(towerHeight_);
        _proj.setup();
        _projectors.push_back(std::move(_proj));
      }

      return _projectors;

    }

    int RingArray::numberOfProjectors() const
    {
      return numberOfProjectors_;
    }

    void RingArray::setNumberOfProjectors(int _numberOfProjectors)
    {
      numberOfProjectors_ = _numberOfProjectors;
    }

    float RingArray::distanceCenter() const
    {
      return distanceCenter_;
    }

    void RingArray::setDistanceCenter(float _distanceCenter)
    {
      distanceCenter_ = _distanceCenter;
    }

    float RingArray::towerHeight() const
    {
      return towerHeight_;
    }

    void RingArray::setTowerHeight(float _towerHeight)
    {
      towerHeight_ = _towerHeight;
    }

    Angle RingArray::pitch() const
    {
      return pitch_;
    }

    void RingArray::setPitch(Angle const& _pitch)
    {
      pitch_ = _pitch;
    }
    
    Angle RingArray::yaw() const
    {
      return yaw_;
    }

    void RingArray::setYaw(Angle const& _yaw)
    {
      yaw_ = _yaw;
    }

    Angle RingArray::fov() const
    {
      return fov_;
    }

    void RingArray::setFov(Angle const& _fov)
    {
      fov_ = _fov;
    }
  }
}

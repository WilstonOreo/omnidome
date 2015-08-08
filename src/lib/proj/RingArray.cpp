#include <omni/proj/RingArray.h>

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
        _proj.setFov(fov_);/*
        auto* _setup = _projectors.setup("PeripheralSetup");


        _projectors.setup*/
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

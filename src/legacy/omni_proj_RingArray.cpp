#include <omni/proj/RingArray.h>

#include <omni/canvas/Interface.hpp>

using namespace omni::proj;

RingArray::RingArray() :
  throw_ratio_(0.8),
  pitch_(20.0),
  tower_height_(0.0),
  distance_from_canvas_(0.5),
  projector_count_(3) {}

std::vector<Projector> RingArray::setup(canvas::Interface const* _canvas) const
{
  float _yawAngleDelta = 360.0 / projector_count();
  float _yaw = 0;
  std::vector<Projector> _projectors;

  for (size_t i = 0; i < projector_count(); ++i)
  {
    Projector _proj;
    _proj.throwRatio(throw_ratio());
    _proj.adjust(_yaw,_canvas->radius()-distance_from_canvas(),tower_height(),0,pitch(),0,0);
    _yaw += _yawAngleDelta;
    _projectors.push_back(_proj);
  }
  return _projectors;
}

void RingArray::additionalParameters(Config& _config) const
{
  using namespace tbd::parameter;
  make(*this,_config,"throw_ratio",
  {
    {"min", "0.1"},
    {"max", "2.0"}
  });
  make(*this,_config,"pitch",
  {
    {"min", "-90.0"},
    {"max", "90.0"}
  });
  make(*this,_config,"tower_height",
  {
    {"min", "-12.0"},
    {"max", "4.0"}
  });
  make(*this,_config,"distance_from_canvas",
  {
    {"min", "0.0"},
    {"max", "10.0"}
  });
  make(*this,_config,"projector_count",
  {
    {"min", "2"},
    {"max", "8"}
  });
}

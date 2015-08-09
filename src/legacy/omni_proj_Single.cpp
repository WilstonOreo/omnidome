#include <omni/proj/Single.h>

#include <omni/canvas/Interface.hpp>

using namespace omni::proj;

Single::Single() :
  throw_ratio_(0.5),
  pitch_(0.0),
  tower_height_(0.0)
{}

std::vector<Projector> Single::setup(canvas::Interface const*) const
{
  std::vector<Projector> _projectors;
  Projector _proj;
  _proj.throwRatio(throw_ratio());
  _proj.adjust(0,0,tower_height(),0,pitch(),0,0);
  _projectors.push_back(_proj);
  return _projectors;
}

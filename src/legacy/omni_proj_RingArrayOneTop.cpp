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

#include <omni/proj/RingArrayOneTop.h>

#include <omni/canvas/Interface.hpp>

using namespace omni::proj;

RingArrayOneTop::RingArrayOneTop() :
  throw_ratio_(0.8),
  pitch_(20.0),
  top_pitch_(90.0),
  tower_height_(0.0),
  distance_from_canvas_(0.5),
  projector_count_(4)
{}

std::vector<Projector> RingArrayOneTop::setup(canvas::Interface const* _canvas) const
{
  float _yawAngleDelta = 360.0 / (projector_count()-1);
  float _yaw = 0;
  std::vector<Projector> _projectors;

  Projector _proj;
  _proj.throwRatio(throw_ratio());

  for (size_t i = 0; i < projector_count()-1; ++i)
  {
    _proj.adjust(_yaw,_canvas->radius()-distance_from_canvas(),tower_height(),0,
                 pitch(),0,0);
    _yaw += _yawAngleDelta;
    _projectors.push_back(_proj);
  }

  _proj.adjust(0,0,tower_height(),0,top_pitch(),0,0);
  _projectors.push_back(_proj);

  return _projectors;
}

void RingArrayOneTop::additionalParameters(Config& _config) const
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
  make(*this,_config,"top_pitch",
  {
    {"min", "0.0"},
    {"max", "90.0"}
  });
  make(*this,_config,"tower_height",
  {
    {"min", "-2.0"},
    {"max", "4.0"}
  });
  make(*this,_config,"distance_from_canvas",
  {
    {"min", "0.0"},
    {"max", "10.0"}
  });
  make(*this,_config,"projector_count",
  {
    {"min", "4"},
    {"max", "8"}
  });
}

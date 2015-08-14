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

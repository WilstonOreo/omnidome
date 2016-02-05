/******************************************************************
    This file is part of DomeSimulator.

    DomeSimulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DomeSimulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DomeSimulator.  If not, see <http://www.gnu.org/licenses/>.

    DomeSimulator is free for non-commercial use. If you want to use it 
    commercially, you should contact the author 
    Michael Winkelmann aka Wilston Oreo by mail:
    me@wilstonoreo.net
**************************************************************************/

#define MAP_DYNAMIC 
#define MAP_SPHERICAL 0
#define MAP_FISHEYE 1
#define MAP_CUBEMAP 2

uniform sampler2D texture;

uniform vec2 resolution;

uniform float yaw;
uniform float pitch;
uniform float roll;

uniform float height;
uniform float width;

#include "direction.h"

void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  
  vec3 raydir;

  if (direction(gl_FragCoord.xy,resolution,deg2rad(pitch),deg2rad(yaw),deg2rad(roll),raydir) < 0.0)
  {
      return;
  }

  float t = 1.0 / dot(raydir,vec3(0.0,0.0,1.0));
  
  vec2 pos = (t * raydir).xy;
  if ((abs(pos.x) > width*0.5) || (abs(pos.y) > height*0.5) || (t < 0.0))
  { 
    return;
  }

  vec2 texCoords = pos / vec2(width,height) * 1.0 + vec2(0.5);
  gl_FragColor = texture2D(texture, texCoords); 
}


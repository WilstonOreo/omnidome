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
#define MAP_ROTATE
#define MAP_SPHERICAL 0
#define MAP_FISHEYE 1
#define MAP_CUBE 2
#define MAP_SPHERICAL_STRIP

uniform sampler2D image;
uniform sampler2D image_top;
uniform sampler2D image_bottom;

uniform vec2 resolution;

uniform float yaw;
uniform float pitch;
uniform float roll;

uniform float height;

uniform float offset_x;
uniform float offset_y;
uniform float offset_z;

uniform float radius;
/*uniform float top_radius;
uniform float bottom_radius;
uniform float slice_begin_angle;
uniform float slice_end_angle;
*/
vec3 offset;

#include "direction.h"

vec2 cylinder_texCoords(in vec3 center, in vec3 iPoint, in int side)
{
  if (side == 0)
  {
    vec3 normal = iPoint - center;
    normal.z = 0.0;
    normal = normalize(normal);
    float s = fract(atan(normal.x,normal.y) / (2.0*PI) - 0.5);
    float t = fract((iPoint.z - center.z) / height + 0.5);
    return vec2(s,t);  
  } else
  if (side == 1)
  {
    return vec3((iPoint - center) * radius *0.5 + 0.5).xy;
  }
  if (side == 2)
  {
    return vec3((iPoint - center) * radius *0.5 + 0.5).xy;
  }

  return vec2(0.0,0.0);
}

int cylinder_intersection(in vec3 center, in vec3 raydir, out vec3 iPoint)
{
  vec2 o = -center.xy;
  float a = dot(raydir.xy,raydir.xy);
  float b = 2.0 * dot(o,raydir.xy);
  float c = dot(o,o) - radius*radius;
  float t = solveQuadraticEquation(a,b,c);
  if (t < radius / 1000.0) return -1;

  iPoint = t * raydir;
  float h = center.z - height * 0.5;
  if (iPoint.z < h)
  {
    float t_cap = h / raydir.z;
    if (t_cap > t || t_cap < 0.0) return -1;
    iPoint = vec3(t_cap * raydir.xy,h); 
    return 1;
  }
  h = center.z + height * 0.5;
  if (iPoint.z > h)
  {
    float t_cap = h / raydir.z;
    if (t_cap > t || t_cap < 0.0) return -1;
    iPoint = vec3(t_cap * raydir.xy,center.z + height); 
    return 2;
  }
  return 0;
}

vec4 cylinder_texturize(in vec3 offset, in vec3 iPoint, in int side)
{
  vec2 texCoords = cylinder_texCoords(offset,iPoint,side);
  
  if (side == 0)
  {
    return texture2D(image, texCoords); 
  } else
  if (side == 1)
  {
    return texture2D(image_top, texCoords); 
  } else
  if (side == 2)
  {
    return texture2D(image_bottom, texCoords); 
  }
}

void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  vec3 raydir;
  if (direction(gl_FragCoord.xy,resolution,deg2rad(pitch),deg2rad(yaw),deg2rad(roll),raydir) < 0.0)
  {
    return;
  }  
  offset = vec3(offset_x,offset_y,offset_z);
  vec3 iPoint;
  int side = cylinder_intersection(offset,raydir,iPoint);
  if (side < 0)
  {
    return;
  }
  
  gl_FragColor = cylinder_texturize(offset,iPoint,side);
}


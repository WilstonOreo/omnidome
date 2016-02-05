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

uniform float offset;
uniform float top;
uniform float bottom;

#include "direction.h"




vec2 cylinder_texCoords(in vec3 center, in vec3 iPoint)
{
  vec3 normal = iPoint - center;
  normal.z = 0.0;
  normal = normalize(normal);
  float s = fract(atan(normal.x,normal.y) / (2.0*PI) - 0.5);
  float t = fract((iPoint.z - bottom + 0.5) / (top - bottom));
  return vec2(s,t);  
}

float cylinder_intersection(in vec3 raydir, out vec3 iPoint)
{
  float a = dot(raydir.xy,raydir.xy);
  float c = - 1.0;
  float t = solveQuadraticEquation(a,0.0,c);
  if (t < 0.0001) return -1.0;

  iPoint = t * raydir;
  if (iPoint.z < bottom - 0.5 || 
      iPoint.z > top - 0.5) return -1.0;
  return 1.0;
}


void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  vec2 texCoords = gl_FragCoord.xy / resolution;

  #ifdef MAP_SPHERICAL
  texCoords.y = 1.0 - ( texCoords.y - bottom) / (top - bottom);
  #endif
  #ifdef MAP_FISHEYE
  float bottom = 1.0 - bottom;
  float top = 1.0 - top;
  vec2 t = (texCoords - 0.5)*2.0;
  float theta = atan(t.x,t.y);
  float r = 1.0 - (sqrt(t.x*t.x + t.y*t.y) - bottom) / (top - bottom);
  texCoords.s = fract(theta/3.14159*0.5 + offset); 
  texCoords.t = r; 
  #endif
  #ifdef MAP_CUBEMAP
  float bottom = 1.0 - bottom;
  float top = 1.0 - top;
  vec3 raydir;
  direction(gl_FragCoord.xy,resolution,raydir);
  vec3 iPoint;
  if (cylinder_intersection(raydir,iPoint) < 0.0)
  {
    return;
  }
  texCoords = cylinder_texCoords(vec3(0,0,0),iPoint);
  texCoords.x += offset;
  texCoords.x = fract(texCoords.x);
  #endif

  
  if ((texCoords.y > 1.0) || (texCoords.y < 0.0) || 
      (texCoords.x > 1.0) || (texCoords.x < 0.0)) return;
  
//  vec2 texCoords = pos / vec2(width,height) * 1.0 + vec2(0.5);
  gl_FragColor = texture2D(texture, texCoords); 
}
 

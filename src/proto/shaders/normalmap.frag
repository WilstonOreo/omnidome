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

uniform sampler2D image; // Texture image
uniform sampler2D normal_map;
uniform bool map_mirror_horizontal;
uniform bool map_mirror_vertical;

vec3 normal;

const float PI = 3.14159265358979323846264;

/// Convert degrees to radians
float deg2rad(in float deg)
{
  return deg * PI / 180.0;
}

#ifdef MAP_DYNAMIC
uniform int map_mode;
#endif

/// Calculates the rotation matrix of a rotation around Z axis with an angle in radians
mat3 rotateAroundZ( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(  c, -s,0.0,
                s,  c,0.0,
              0.0,0.0,1.0);
}

#ifdef MAP_ROTATE
uniform float map_yaw; // Any value allowed
uniform float map_pitch; // Any value allowed
uniform float map_roll; // Any value allowed

/// Calculates the rotation matrix of a rotation around X axis with an angle in radians
mat3 rotateAroundX( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(1.0,0.0,0.0,
              0.0,  c, -s,
              0.0,  s,  c);
}

/// Calculates the rotation matrix of a rotation around Y axis with an angle in radians
mat3 rotateAroundY( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(  c,0.0,  s,
              0.0,1.0,0.0,
               -s,0.0,  c);
}

/// Calculate rotation by given yaw and pitch angles (in degrees!)
mat3 rotationMatrix(in float yaw, in float pitch, in float roll)
{
  return rotateAroundZ(deg2rad(yaw)) * 
         rotateAroundY(deg2rad(-pitch)) * 
         rotateAroundX(deg2rad(roll));
}


vec3 map_normal()
{
  return rotationMatrix(map_yaw,map_pitch,map_roll) * normal;
}
#else
#ifdef MAP_YAW
uniform float map_yaw; // Any value allowed
vec3 map_normal()
{
  return normal * rotateAroundZ(map_yaw);
}
#else
vec3 map_normal()
{
  return normal;
}
#endif
#endif

#ifdef MAP_SPHERICAL

float map_spherical(out vec2 texCoords) 
{
  vec3 normal = map_normal();
  texCoords.s = fract(atan(normal.y,normal.x) / (2.0*PI));
  texCoords.t = fract(acos(normal.z) / PI);
  return 1.0;
}
#endif

#ifdef MAP_FISHEYE
float map_fisheye(out vec2 texCoords)
{
  vec3 n = map_normal();
  float phi = atan(sqrt(n.x*n.x + n.y*n.y),n.z);
  float r =  phi / PI * 2.0;
  if ((r > 1.0) || (r <= 0.0)) return -1.0;
  float theta = atan(n.x,n.y);
  texCoords.s = fract(0.5 * (1.0 + r* cos(theta)));
  texCoords.t = fract(0.5 * (1.0 + r * sin(theta)));
  return 1.0; 
}
#endif
#ifdef MAP_CUBE
float map_cube(out vec2 texCoords)
{
  vec3 n = map_normal();
  float sc, tc, ma;
  float eps =  -0.015;
  float _off = 0.0;
  //n = n.yzx;

  if ((abs(n.x) >= abs(n.y)) && (abs(n.x) >= abs(n.z)))
  {
    sc = (n.x > 0.0) ? -n.y : n.y;
    tc = n.z;
    ma = n.x;
    _off += (n.x < 0.0) ? 0.0/6.0 : 2.0/6.0; // EAST / WEST
  } else
  if ((abs(n.y) >= abs(n.z)))
  {
    sc = (n.y < 0.0) ? -n.x : n.x;
    tc = n.z;
    ma = n.y;
    _off += (n.y < 0.0) ? 1.0/6.0 : 3.0/6.0; // NORTH / SOUTH
  } else
  {
    sc = (n.z > 0.0) ? n.y : n.y;
    tc = (n.z > 0.0) ? n.x : -n.x;
//    tc = -n.x; 
    ma = n.z;
    _off = (n.z < 0.0) ? 4.0/6.0 : 5.0 / 6.0;  // TOP / BOTTOM
  }
  texCoords = vec2(sc/(12.0 - eps)/abs(ma) + 0.5/6.0 + _off,0.5+tc/(2.0 - eps)/abs(ma)) ;
  return 1.0;
}
#endif

#ifdef MAP_DYNAMIC
float mapping(out vec2 texCoords)
{ 
#ifdef MAP_SPHERICAL 
  if (map_mode == MAP_SPHERICAL)
  {
    return map_spherical(texCoords);
  }
#endif
#ifdef MAP_FISHEYE
  if (map_mode == MAP_FISHEYE)
  {
    return map_fisheye(texCoords);
  }
#endif 
#ifdef MAP_CUBE
  if (map_mode == MAP_CUBE)
  {
    return map_cube(texCoords);
  }
#endif
  return -1.0;
}
#endif
#ifndef MAP_DYNAMIC
#ifdef MAP_SPHERICAL
float mapping(out vec2 texCoords)
{
  return map_spherical(texCoords);
}
#endif 
#ifdef MAP_FISHEYE
float mapping(out vec2 texCoords)
{
  return map_fisheye(texCoords);
}
#endif 
#ifdef MAP_CUBE
float mapping(out vec2 texCoords)
{
  return map_cube(texCoords);
}
#endif 
#endif

void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,1.0);
  const float ONE_THIRD = 1.0 / 3.0;
  vec2 uv = gl_TexCoord[0].st;
  uv.y = 1.0 - uv.y;
  if (uv.t >= 0.9995)
  {
    uv.t = 0.9995;
  }
   uv *= vec2(1.0,ONE_THIRD);
  vec3 n = texture2D(normal_map,uv).xyz*2.0-1.0 + texture2D(normal_map,uv  + vec2(0.0,ONE_THIRD)).xyz*2.0/255.0 - 1.0 / 255.0;

  if (length(n) <= 0.01) 
  {
    return;
  }
  normal = normalize(n);
  
  vec2 texCoords;
  if (mapping(texCoords) < 0.0)
  {
    return;
  }

  if (!map_mirror_horizontal)
  {
    texCoords.s = 1.0 - texCoords.s; 
  }
  if (map_mirror_vertical)    
  {
    texCoords.t = 1.0 - texCoords.t; 
  }

  float alpha = texture2D(normal_map,uv + vec2(0.0,2.0 * ONE_THIRD)).r;
  gl_FragColor = vec4(texture2D(image, texCoords).rgb*alpha,1.0); 
}


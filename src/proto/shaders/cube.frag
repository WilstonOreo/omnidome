#define MAP_DYNAMIC
#define MAP_ROTATE
#define MAP_SPHERICAL 0
#define MAP_FISHEYE 1
#define MAP_CUBE 2
#define MAP_SPHERICAL_STRIP

uniform sampler2D image_top;
uniform sampler2D image_bottom;
uniform sampler2D image_left;
uniform sampler2D image_right;
uniform sampler2D image_front;
uniform sampler2D image_back;

uniform vec2 resolution;

uniform float yaw;
uniform float pitch;
uniform float roll;

uniform float dimensions_x;
uniform float dimensions_y;
uniform float dimensions_z;

uniform float offset_x;
uniform float offset_y;
uniform float offset_z;

#include "direction.h"

vec3 cube[2];
vec3 offset;
vec3 dimensions;

int sgn(float p)
{
  return (p > 0.0) ? 0 : 1;
}

#define SWAP(type,a,b)\
  do { type t = a; a = b; b = t; } while (false);

/// Parameters: rd = ray direction, iPoint = intersection point
int cube_intersection(in vec3 rd, out vec3 iPoint)  
{
  float tmin, tmax, tymin, tymax, tzmin, tzmax;

  int side_near = 0;
  int side_far = 0;
  tmin = cube[sgn(rd.x)].x / rd.x;
  tmax = cube[sgn(-rd.x)].x / rd.x;
  tymin = cube[sgn(rd.y)].y / rd.y;
  tymax = cube[sgn(-rd.y)].y / rd.y;
  if ( (tmin > tymax) || (tymin > tmax) ) 
    return -1;
  if (tymin > tmin)
  {
    tmin = tymin;
    side_near = 1;
  }
  if (tymax < tmax)
  {
    tmax = tymax;
    side_far = 1;
  }
  tzmin = cube[sgn(rd.z)].z / rd.z;
  tzmax = cube[sgn(-rd.z)].z / rd.z;
  if ( (tmin > tzmax) || (tzmin > tmax) ) 
    return -1;
  if (tzmin > tmin)
  {
    tmin = tzmin;
    side_near = 2;
  }
  if (tzmax < tmax)
  {
    tmax = tzmax;
    side_far = 2;
  }

  if (tmax < 0.0) return -1;

  if (tmax < tmin) 
  {
    SWAP(float,tmin,tmax);
    SWAP(int,side_far,side_near);
  }
  iPoint = rd * tmax; 
  
  return side_far;
}

vec4 cube_texturize(vec3 iPoint, int side)
{
  vec3 d = (iPoint - offset) / dimensions;
  
  if (side == 0)
  {
    return texture2D(d.x < 0.0 ? image_left : image_right,d.yz + 0.5);
  } else
  if (side == 1)
  {
    return texture2D(d.y < 0.0 ? image_back : image_front,d.xz + 0.5);
  } else
  if (side == 2)
  {
    return texture2D(d.z < 0.0 ? image_bottom : image_top,d.xy + 0.5);
  } 
  return vec4(0.0);
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
  dimensions = vec3(dimensions_x,dimensions_y,dimensions_z);

  vec3 halfdim = dimensions * 0.5;
  cube[0] = -halfdim + offset;
  cube[1] = halfdim + offset;

  vec3 iPoint;
  int side = cube_intersection(raydir,iPoint);  
  if (side < 0)
  {
    return;
  }

  gl_FragColor = cube_texturize(iPoint,side);
}


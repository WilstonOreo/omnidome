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

////////// shaders/canvas.h //////////
#define HALFDOME
//#define FULLDOME

////////// shaders/projector.h //////////
////////// shaders/projection.h //////////
////////// shaders/util.h //////////

const float PI = 3.14159265358979323846264;

struct Ray
{
  vec3 org;
  vec3 dir;
};

/// Convert degrees to radians
float deg2rad(in float deg)
{
  return deg * PI / 180.0;
}

/// Convert degrees to radians
float rad2deg(in float rad)
{
  return rad / PI * 180.0;
}

float sqr(in float a)
{
  return a*a;
}

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

/// Calculates the rotation matrix of a rotation around Z axis with an angle in radians
mat3 rotateAroundZ( in float angle )
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3(  c, -s,0.0,
                s,  c,0.0,
              0.0,0.0,1.0);
}

/// Calculate rotation by given yaw and pitch angles (in degrees!)
mat3 rotationMatrix(in float yaw, in float pitch, in float roll)
{
  return rotateAroundZ(deg2rad(yaw)) * 
         rotateAroundY(deg2rad(-pitch)) * 
         rotateAroundX(deg2rad(roll));
}

float solveQuadraticEquation(
    in float a,
    in float b,
    in float c)
{
  float disc = b*b - 4.0 * a * c;
  if (disc < 0.0) return -1.0;

  float distSqrt = sqrt(disc);
  float q;
  if (b < 0.0)
  {
    q = (-b - distSqrt)*0.5;
  }
  else
  {
    q = (-b + distSqrt)*0.5;
  }
  if (q == 0.0) return -1.0;

  float t = c / q;
  if (t < 0.0) 
    t = q / a;

  return t;
}


struct Projector
{
  float fov;
  float aspect_ratio;
  vec3 offset;
  mat3 matrix;
};

Projector Projector_construct(
    float fov,
    float aspect_ratio,
    vec3 offset,
    float yaw,
    float pitch,
    float roll)
{
  Projector proj;
  proj.fov = fov;
  proj.aspect_ratio = aspect_ratio;
  proj.offset = offset;
  proj.matrix = rotationMatrix(yaw,pitch,roll);
  return proj;
}

void Projector_coordinateSystem(
  in Projector proj,
  out vec3 look_at,
  out vec3 side,
  out vec3 up)
{
  look_at = proj.matrix * vec3(1.0,0.0,0.0);
  side = proj.matrix * vec3(0.0,1.0,0.0);
  up = proj.matrix * vec3(0.0,0.0,1.0);
}

struct Frustum
{
  vec3 eye;
  vec3 look_at;
  vec3 top_left; 
  vec3 top_right;
  vec3 bottom_left;
  vec3 bottom_right;
};

Frustum Frustum_construct(in Projector proj)
{
  Frustum frustum;
  float a = proj.fov / 2.0;
  float width = tan(deg2rad(a));
  float height = width * proj.aspect_ratio;
  frustum.eye = proj.offset;
  frustum.top_left = proj.matrix * vec3(1.0,-width,height);
  frustum.top_right = proj.matrix * vec3(1.0,width,height);
  frustum.bottom_left = proj.matrix * vec3(1.0,-width,-height);
  frustum.bottom_right = proj.matrix * vec3(1.0,width,-height);
  frustum.look_at = proj.matrix * vec3(1.0,0.0,0.0);
  return frustum;
}

Frustum Frustum_construct(in Projector proj, 
    float squeezeLeft, 
    float squeezeRight, 
    float squeezeTop, 
    float squeezeBottom) 
{
  Frustum frustum;
  float a = proj.fov / 2.0;
  float width = tan(deg2rad(a));
  float height = width * proj.aspect_ratio;
  frustum.eye = proj.offset;
  frustum.top_left = proj.matrix * vec3(1.0,-width + squeezeLeft,height - squeezeBottom);
  frustum.top_right = proj.matrix * vec3(1.0,width - squeezeRight,height - squeezeBottom);
  frustum.bottom_left = proj.matrix * vec3(1.0,-width + squeezeLeft,-height + squeezeTop);
  frustum.bottom_right = proj.matrix * vec3(1.0,width - squeezeRight,-height + squeezeTop);
  frustum.look_at = proj.matrix * vec3(1.0,0.0,0.0);
  return frustum;
}

/// Tests if a point intersects frustum. Returns minimum distance from point to frustum
/// If distance > 0, there is an intersection
float Frustum_intersection(in Frustum frustum, in vec3 point, float offset)
{
  vec3 o = point - frustum.eye;
  /// Normals of frustum required for calculating intersection
  vec3 n_top = -normalize(cross(frustum.top_left,frustum.top_right)); // Top normal
  vec3 n_bottom = normalize(cross(frustum.bottom_left,frustum.bottom_right)); // Bottom normal
  vec3 n_left = normalize(cross(frustum.top_left,frustum.bottom_left)); // Left normal
  vec3 n_right = -normalize(cross(frustum.top_right,frustum.bottom_right)); // Right normal

  /// Distances to frustum planes
  float d = dot(o,frustum.look_at); 
  float d_top = dot(o,n_top); 
  float d_bottom = dot(o,n_bottom);
  float d_left = dot(o,n_left);
  float d_right = dot(o,n_right);
  
  if (d < offset) return -100000000.0;
  if ((d_top < offset) && (d_bottom < offset) && (d_left < offset) && (d_right < offset))
  {
    return -1000000.0; // min(min(-d_top,-d_bottom),min(-d_left,-d_right));
  } 

  /// Return minimum distance
  return min(min(d_top,d_bottom),min(d_left,d_right));
}

float Frustum_intersection(in Frustum frustum, in vec3 point)
{
  return Frustum_intersection(frustum,point,0.0);
}

/// Calculate point on projector screen by given screen coordinates
vec3 Frustum_pointOnScreen(in Frustum frustum, in vec2 screenCoord)
{
  return mix(mix(frustum.top_left,frustum.top_right,screenCoord.x),
             mix(frustum.bottom_left,frustum.bottom_right,screenCoord.x),screenCoord.y);
}

Ray Frustum_ray(in Frustum frustum, in vec2 screenCoord)
{
  Ray ray;
  ray.org = frustum.eye; 
  ray.dir = Frustum_pointOnScreen(frustum,screenCoord);
  return ray;
}


uniform float proj_fov; // 68.2 is default
uniform float proj_aspect_ratio; // 0.75 is default
uniform float proj_distance_center;
uniform float proj_shift;
uniform float proj_tower_height;

/// Range from -90.0 to 90.0
uniform float proj_yaw;
uniform float proj_delta_yaw;
uniform float proj_pitch;
uniform float proj_roll;

Projector proj;

void proj_params()
{
  float yaw = proj_yaw;
  float theta = deg2rad(yaw);
  vec2 p = proj_distance_center*vec2(-cos(theta),sin(theta));
  float ct = cos(-theta), st = sin(-theta);
  vec2 shiftVec = normalize(vec2(st,ct)) * proj_shift;  
  vec3 pos =  vec3(p + shiftVec,- proj_tower_height);
  proj = Projector_construct(proj_fov,proj_aspect_ratio,pos,proj_yaw + proj_delta_yaw,proj_pitch,proj_roll);
}

Ray proj_frustum_ray(in vec2 screenCoord)
{
  Frustum frustum = Frustum_construct(proj);
  return Frustum_ray(frustum,screenCoord);
}

#ifdef FULLDOME
////////// shaders/fulldome.h //////////
////////// shaders/spherical.h //////////
////////// shaders/util.h //////////

float isect_squeezed_sphere(
    in Ray ray,
    in float radius,
    in float z_radius,
    in vec3 center,
    out vec3 iPoint)
{
  vec3 o = ray.org - center;
  float invRadius = 1.0 / radius;
  float invZRadius = 1.0 / z_radius;
  vec3 o_T = vec3(o.x * invRadius,o.y * invRadius,o.z * invZRadius);
  vec3 d_T = vec3(ray.dir.x * invRadius,ray.dir.y * invRadius,ray.dir.z * invZRadius);

  float a = dot(d_T,d_T);
  float b = 2.0 * dot(d_T,o_T);
  float c = dot(o_T,o_T) - 1.0;
  float t = solveQuadraticEquation(a,b,c);
  if (t < radius / 1000.0) return -1.0;

  iPoint = ray.org + t * ray.dir;
  return 1.0;
}


/// Return 1.0 if ray intersects sphere, otherwise -1.0
float isect_sphere(
    in Ray ray,
    in vec3 center, 
    in float radius, 
    out vec3 iPoint)
{
  vec3 o = ray.org - center;
  float a = dot(ray.dir,ray.dir);
  float b = 2.0 * dot(ray.dir,o);
  float c = dot(o,o) - sqr(radius);
  float t = solveQuadraticEquation(a,b,c);
  if (t < radius / 1000.0) return -1.0;

  iPoint = ray.org + t * ray.dir;
  return 1.0;
}

/// Return 1.0 if ray intersects sphere, otherwise -1.0
float isect_sphere(
    in Ray ray,
    in float radius, 
    out vec3 iPoint)
{
  float a = dot(ray.dir,ray.dir);
  float b = 2.0 * dot(ray.dir,ray.org);
  float c = dot(ray.org,ray.org) - sqr(radius);
  float t = solveQuadraticEquation(a,b,c);
  if (t < radius * 0.001) return -1.0;

  iPoint = ray.org + t * ray.dir;
  return 1.0;
}

uniform float canvas_diameter;
uniform float canvas_distance_to_pole;
uniform float canvas_distance_to_equator;

// A dome covering more than a half sphere with z squeezing 
// (Inflatable dome)
struct Canvas
{
  vec3 normal;
  vec3 iPoint;
  float diameter;
  float z_radius;
  float z;
} canvas;

void canvas_params() 
{
  canvas.diameter = canvas_diameter;
  canvas.z = sqrt(abs(sqr(canvas_distance_to_equator) - sqr(dome._radius))); 
  canvas.z_radius = canvas_distance_to_pole - dome.z;
}

float canvas_intersection(in Ray ray, out vec3 iPoint)
{
  return isect_squeezed_sphere(ray,canvas.diameter * 0.5005,_z_radius,canvas_center(),iPoint);
}

vec3 canvas_center()
{
  return vec3(0.0,0.0,canvas.z);
}

vec3 canvas_normal()
{
  return normalize(canvas.iPoint - canvas_center());
}

#define MAP_ROTATE
#define MAP_SPHERICAL 0
#define MAP_CUBE 2
#endif

#ifdef HALFDOME
////////// shaders/halfdome.h //////////
////////// shaders/spherical.h //////////

// A dome which is a half sphere without z squeezing
// (Planetarium, geodesic dome)
uniform float halfdome_diameter;

struct Canvas
{
  vec3 normal;
  vec3 iPoint;
  float diameter;
} canvas;


void canvas_params() 
{
  canvas.diameter = halfdome_diameter;
}

vec3 canvas_center()
{
  return vec3(0.0,0.0,0.0);
}

/// Return 1.0 if ray intersects sphere, otherwise -1.0
float isect_sphere(
    in Ray ray,
    in float radius, 
    out vec3 iPoint)
{
  float a = dot(ray.dir,ray.dir);
  float b = 2.0 * dot(ray.dir,ray.org);
  float c = dot(ray.org,ray.org) - sqr(radius);
  float t = solveQuadraticEquation(a,b,c);
  if (t < radius * 0.001) return -1.0;

  iPoint = ray.org + t * ray.dir;
  return 1.0;
}

float canvas_intersection(in Ray ray, out vec3 iPoint)
{
  return isect_sphere(ray,canvas.diameter * 0.5005,iPoint);
}


vec3 canvas_normal()
{
  return normalize(canvas.iPoint);
}

#define MAP_ROTATE
#define MAP_SPHERICAL 0
#define MAP_FISHEYE 1
#define MAP_CUBE 2
#endif

float canvas_params(in vec2 screenCoord)
{
  proj_params();
  canvas_params();

  if (canvas_intersection(proj_frustum_ray(screenCoord),canvas.iPoint) < 0.0)
  {
    return -1.0;
  }
  
  canvas.normal = canvas_normal();
  return 1.0;
}

////////// shaders/mapping.h //////////
uniform sampler2D map_texture; // Texture image
uniform vec2 resolution;

#ifdef MAP_DYNAMIC
uniform float map_mode;
#endif

#ifdef MAP_ROTATE
uniform float map_yaw; // Any value allowed
uniform float map_pitch; // Any value allowed
uniform float map_roll; // Any value allowed

vec3 map_normal()
{
  return rotationMatrix(map_yaw,map_pitch,map_roll) * canvas.normal;
}
#else
#ifdef MAP_YAW
uniform float map_yaw; // Any value allowed
vec3 map_normal()
{
  return canvas.normal * rotateAroundZ(map_yaw);
}
#else
vec3 map_normal()
{
  return canvas.normal;
}
#endif
#endif

#ifdef MAP_SPHERICAL_STRIP
uniform float map_strip_top; // 0.0
uniform float map_strip_bottom; // 1.0
#endif

#ifdef MAP_SPHERICAL
float map_spherical(out vec2 texCoords) 
{
  vec3 normal = map_normal();
  texCoords.s = 1.0 - fract(atan(normal.x,normal.y) / (2.0*PI) - 0.5);
  texCoords.t = 1.0 - fract(acos(normal.z) / PI);

  texCoords.t = fract(texCoords.y);
  texCoords.s = fract(texCoords.x);
#ifdef MAP_SPHERICAL_STRIP
  if (texCoords.y < map_strip_top || texCoords.y > map_strip_bottom)
  {
    return -1.0;
  }
  texCoords.t = ( texCoords.y - map_strip_top) / (map_strip_bottom - map_strip_top);
#endif
  return 1.0;
}
#endif

#ifdef MAP_FISHEYE
float map_fisheye(out vec2 texCoords)
{
  vec3 n = map_normal();
  float phi = atan(sqrt(n.x*n.x + n.y*n.y),-n.z);
  float r =  phi / PI * 2.0;
  if ((r > 1.0) || (r <= 0.0)) return -1.0;
  float theta = atan(n.x,n.y);
  texCoords.s = fract(0.5 * (1.0 + r* sin(theta)));
  texCoords.t = fract(0.5 * (1.0 + r * cos(theta)));
  return 1.0; 
}
#endif
#ifdef MAP_CUBE
float map_cube(out vec2 texCoords)
{
  vec3 n = map_normal();
  float sc, tc, ma;
  float eps =  -0.02;
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
  int mode = int(map_mode);
  
#ifdef MAP_SPHERICAL 
  if (mode == MAP_SPHERICAL)
  {
    return map_spherical(texCoords);
  }
#endif
#ifdef MAP_FISHEYE
  if (mode == MAP_FISHEYE)
  {
    return map_fisheye(texCoords);
  }
#endif 
#ifdef MAP_CUBE
  if (mode == MAP_CUBE)
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
  if (canvas_params(gl_TexCoord[0].st) < 0.0)
  {
    return;
  }
  vec2 texCoords = vec2(0.0,0.0);
  if (mapping(texCoords) < 0.0)
  {
    return;
  }

  gl_FragColor = texture2D(map_texture, texCoords); 
}



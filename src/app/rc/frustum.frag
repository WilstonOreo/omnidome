#version 120
 varying vec3 vVertexPosition;
 varying vec3 vVertexNormal;
uniform vec3 eye;
uniform vec3 look_at;
uniform vec3 top_left; 
uniform vec3 top_right;
uniform vec3 bottom_left;
uniform vec3 bottom_right;

/// Color of frustum border
uniform vec4 color;

#define MAP_SPHERICAL 1
#define MAP_FISHEYE 2
#define MAP_CUBE 3
uniform int map_mode;
uniform int map_only;

uniform float map_yaw; // Any value allowed
uniform float map_pitch; // Any value allowed
uniform float map_roll; // Any value allowed

uniform sampler2D texture;

uniform float tex_alpha;
uniform int no_borders;


const float PI = 3.14159265358979323846264;

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


/// Tests if a point intersects frustum. Returns minimum distance from point to frustum
/// If distance > 0, there is an intersection
float frustum_intersection(in vec3 point)
{
  vec3 o = point - eye;
  /// Normals of frustum required for calculating intersection
  vec3 n_top = -normalize(cross(top_left,top_right)); // Top normal
  vec3 n_bottom = normalize(cross(bottom_left,bottom_right)); // Bottom normal
  vec3 n_left = normalize(cross(top_left,bottom_left)); // Left normal
  vec3 n_right = -normalize(cross(top_right,bottom_right)); // Right normal

  /// Distances to frustum planes
  float d = dot(o,look_at); 
  float d_top = dot(o,n_top); 
  float d_bottom = dot(o,n_bottom);
  float d_left = dot(o,n_left);
  float d_right = dot(o,n_right);
  
  if (d < 0.0) return -100000000.0;
  if ((d_top < 0.0) && (d_bottom < 0.0) && (d_left < 0.0) && (d_right < 0.0))
  {
    return -1000000.0; // min(min(-d_top,-d_bottom),min(-d_left,-d_right));
  } 

  /// Return minimum distance
  return min(min(d_top,d_bottom),min(d_left,d_right));
}
vec3 map_normal()
{
  return rotationMatrix(map_yaw,map_pitch,map_roll) * vVertexNormal;
}

float map_spherical(out vec2 texCoords) 
{
  vec3 normal = map_normal();
  texCoords.s =  fract(atan(normal.y,normal.x) / (2.0*PI) );
  texCoords.t =  fract(acos(normal.z) / PI);

  texCoords.t = fract(texCoords.y);
  texCoords.s = fract(texCoords.x);
  return 1.0;
}

float map_fisheye(out vec2 texCoords)
{
  vec3 n = map_normal();
  float phi = atan(sqrt(n.x*n.x + n.y*n.y),n.z);
  float r =  phi / PI * 2.0;
  if ((r > 1.0) || (r <= 0.0)) return -1.0;
  float theta = atan(n.x,n.y);
  texCoords.s = fract(0.5 * (1.0 + r* sin(theta)));
  texCoords.t = fract(0.5 * (1.0 + r * cos(theta)));
  return 1.0; 
}

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

float mapping(out vec2 texCoords)
{
  if (map_mode == MAP_SPHERICAL)
  {
    return map_spherical(texCoords);
  }
  if (map_mode == MAP_FISHEYE)
  {
    return map_fisheye(texCoords);
  }
  if (map_mode == MAP_CUBE)
  {
    return map_cube(texCoords);
  }
  texCoords = gl_TexCoord[0].st;
  texCoords.x += map_yaw / 360.0;
  return 1.0;
}

void map()
{
  vec2 texCoords = gl_TexCoord[0].st;
  if (mapping(texCoords) > 0.0)
  {
    gl_FragColor = vec4(texture2D(texture,texCoords).rgb,tex_alpha);
  }
}

void main()
{
  if (map_only == 1)
  {
    map();
    return;
  }
  float d = frustum_intersection(vVertexPosition);  
  if (d > 0.0)
  {
    if ((d < 0.1) && (no_borders != 0))
    {
      gl_FragColor = color;
    } else
    {
      map();
    }
  } else
  {
    gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  }
//  gl_FragColor = vec4(0.0,1.0,0.0,1.0);
}

#ifndef ISF
uniform int map_mode;
uniform sampler2D image;
uniform sampler2D uvw_map;
uniform float map_yaw;
uniform float map_pitch;
uniform float map_roll;
uniform bool map_flip_vertical;
uniform bool map_flip_horizontal;
uniform vec2 _uvw_map_imgSize;
uniform vec2 _image_imgSize;
#endif

#define MAP_EQUIRECTANGULAR 0
#define MAP_FISHEYE 1
#define MAP_CUBE 2

vec3 uvw;

const float PI = 3.14159265358979323846264;

/// Convert degrees to radians
float deg2rad(in float deg)
{
  return deg * PI / 180.0;
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


vec3 map_uvw()
{
  return rotationMatrix(map_yaw,map_pitch,map_roll) * uvw;
}

#ifdef MAP_EQUIRECTANGULAR

float map_equirectangular(out vec2 texCoords)
{
  vec3 uvw = map_uvw();
  texCoords.s = fract(atan(uvw.y,uvw.x) / (2.0*PI));
  texCoords.t = fract(acos(uvw.z) / PI);
  return 1.0;
}
#endif

#ifdef MAP_FISHEYE
float map_fisheye(out vec2 texCoords)
{
  vec3 n = map_uvw();
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
  vec3 n = map_uvw();
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
    ma = n.z;
    _off = (n.z < 0.0) ? 4.0/6.0 : 5.0 / 6.0;  // TOP / BOTTOM
  }
  texCoords = vec2(sc/(12.0 - eps)/abs(ma) + 0.5/6.0 + _off,0.5+tc/(2.0 - eps)/abs(ma)) ;
  return 1.0;
}
#endif

float mapping(out vec2 texCoords)
{ 
#ifdef MAP_EQUIRECTANGULAR
  if (map_mode == MAP_EQUIRECTANGULAR)
  {
    return map_equirectangular(texCoords);
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


void main()
{
  gl_FragColor = vec4(0.0,0.0,0.0,1.0);

    vec2 s = vec2(1.0,1.0);
    vec2 uv = gl_TexCoord[0].st;

    float f = 1.0 / 1800.0;

    vec2 uvw_upper = vec2(uv.x,(uv.y*s.y)/3.0);
    vec2 uvw_lower = vec2(uv.x,(uv.y*s.y + s.y)/3.0);
    vec2 blendmask = vec2(uv.x,(uv.y*s.y + s.y*2.0) /3.0);

    vec3 lower_n = texture2D(uvw_map,uvw_lower).xyz;
    vec3 upper_n = texture2D(uvw_map,uvw_upper).xyz;

    vec3 n = (upper_n +  lower_n  / 255.0 ) * 2.0 - 1.0;

    // + 1.0 / 255.0 - texture2D(uvw_map,uvw_lower).xyz/127.0 - 0.5;
    
  if (length(n) <= 0.01) 
  {
    return;
  }
  uvw = normalize(n);
 
  vec2 texCoords;
  if (mapping(texCoords) < 0.0)
  {
    return;
  }

  if (map_flip_horizontal)
  {
    texCoords.s = 1.0 - texCoords.s; 
  }
  if (map_flip_vertical)    
  {
    texCoords.t = 1.0 - texCoords.t; 
  }

  float alpha =  texture2D(uvw_map,blendmask).r;
  gl_FragColor = vec4(texture2D(image, texCoords).rgb*alpha,1.0);
}


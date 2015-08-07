varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;

const float PI = 3.14159265358979323846264;

uniform sampler2D texture; // Texture image

uniform float yaw_angle; // Any value allowed
uniform float pitch_angle; // Any value allowed
uniform float roll_angle; // Any value allowed

/// Source from where to get the UVW coordinates
/// 0: Use vertex texture coordinates
/// 1: Get uvw coordinates from uvw_vertex_position
/// 2: Get uvw coordinates from uvw_normal
uniform int uvw_source; 

uniform bool flip_vertical;
uniform bool flip_horizontal;

/// Select whether output texture, texture coordintates
/// or uvw coordinates
/// 0: Output texture
/// 1: Output texture coordinates
/// 2: Output uvw coordinates
uniform int output_mode;

/// Convert degrees to radians
float deg2rad(in float deg)
{
  return deg * PI / 180.0;
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

vec3 get_uvw()
{
  vec3 normal = vec3(0.0,0.0,0.0);
  if (uvw_source == 0) 
  {
    normal = normalize(uvw_normal);
  } else
  if (uvw_source == 1)
  {
    normal = normalize(uvw_vertex_position);
  }

  return normal * rotationMatrix(yaw_angle,pitch_angle,roll_angle);
}

float mapping(in vec3 uvw, out vec2 texCoords);


void main()
{
  vec3 uvw = get_uvw();
  if (output_mode == 2)
  {
    gl_FragColor = vec4(uvw.xyz,1.0);
    return;
  }

  vec2 texCoords;
  if (mapping(uvw,texCoords) > 0.0)
  {
    if (flip_horizontal) texCoords.x = 1.0 - texCoords.x;
    if (flip_vertical) texCoords.y = 1.0 - texCoords.y;
    if (output_mode == 0)
    {
      gl_FragColor = texture2D(texture, texCoords);
    } else if (output_mode == 1)
    {
      gl_FragColor = vec4(texCoords.xy,0.0,1.0);
    }
    return;
  }

  gl_FragColor = vec4(0.0,0.0,0.0,1.0);
}


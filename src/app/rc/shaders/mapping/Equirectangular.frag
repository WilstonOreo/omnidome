
varying vec3 vVertexPosition;

const float PI = 3.14159265358979323846264;

uniform float strip_top; // 0.0
uniform float strip_bottom; // 1.0

uniform sampler2D texture; // Texture image

uniform float yaw_angle; // Any value allowed
uniform float pitch_angle; // Any value allowed
uniform float roll_angle; // Any value allowed

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

vec3 get_normal()
{
  vec3 normal = normalize(vVertexPosition);
  return normal * rotationMatrix(yaw_angle,pitch_angle,roll_angle);
}




float equirectangular(in vec3 normal, out vec2 texCoords) 
{
  texCoords.s = fract(atan(normal.y,normal.x) / (2.0*PI));
  texCoords.t = 1.0 - fract(acos(normal.z) / PI);

  if (texCoords.y < strip_bottom || texCoords.y > strip_top)
  {
    return -1.0;
  }
  texCoords.t = ( texCoords.y - strip_bottom) / (strip_top - strip_bottom);
  
  return 1.0;
}

void main()
{
  vec3 normal = get_normal();
  vec2 texCoords;
  if (equirectangular(normal,texCoords) > 0.0)
  {
    gl_FragColor = texture2D(texture, texCoords);
    return;
  }

  gl_FragColor = vec4(0.0,0.0,0.0,1.0);
}


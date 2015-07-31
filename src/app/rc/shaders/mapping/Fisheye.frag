
varying vec3 vVertexPosition;

const float PI = 3.14159265358979323846264;

uniform float stretch; // 0.0
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

float fisheye(in vec3 n, out vec2 texCoords) 
{
  float phi = atan(sqrt(n.x*n.x + n.y*n.y),n.z);
  float r =  phi / PI * 2.0 / (stretch + 1.0);
  if ((r > 1.0) || (r <= 0.0)) return -1.0;
  float theta = atan(n.x,n.y);
  texCoords.s = fract(0.5 * (1.0 + r* sin(theta)));
  texCoords.t = fract(0.5 * (1.0 + r * cos(theta)));
  return 1.0; 
}

void main()
{
  vec3 normal = get_normal();
  vec2 texCoords;
  if (fisheye(normal,texCoords) > 0.0)
  {
    gl_FragColor = texture2D(texture, texCoords);
    return;
  }

  gl_FragColor = vec4(0.0,0.0,0.0,1.0);
}


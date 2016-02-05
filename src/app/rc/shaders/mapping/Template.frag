varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;
varying mat4 projection_matrix;

const float PI = 3.14159265358979323846264;

uniform sampler2D texture; // Texture image

uniform float yaw_angle; // Any value allowed
uniform float pitch_angle; // Any value allowed
uniform float roll_angle; // Any value allowed

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


/*
float sphereIntersection(out vec3 uvw) {
    vec3 n = uvw_normal;
    vec3 oc = normalize(uvw_vertex_position - offset);
    float b = 2.0 * dot(n, oc);
    float c = dot(oc, oc) - 1.0;
    float disc = b * b - 4.0 * c;

    if (disc < 0.0)
        return -1.0;

    // compute q as described above
    float q;
    if (b < 0.0)
        q = (-b - sqrt(disc))/2.0;
    else
        q = (-b + sqrt(disc))/2.0;

    float t0 = q;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1) {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < -0.0001)
        return -1.0;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < -0.001) {
        uvw = oc + t1 * n;
    } else {
        uvw = oc + t0 * n;
    }

    return 1.0;
}
*/


float intersection(out vec3 uvw);

float mapping(in vec3 uvw, out vec2 texCoords);

void main()
{
  //vec3 uvw = gl_TexCoord[0].xyz * rotationMatrix(yaw_angle,pitch_angle,roll_angle);
  vec3 uvw = uvw_vertex_position;
  if (intersection(uvw) < 0.0) {
      discard;
  }

  uvw = uvw * rotationMatrix(yaw_angle,pitch_angle,roll_angle);

  if (output_mode == 2) // Output UVW map only
  {
    gl_FragColor = vec4(0.5*(normalize(uvw.xyz) + vec3(1.0)),1.0);
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
    } else if (output_mode == 1) // Output texture coordinates
    {
      gl_FragColor = vec4(texCoords.xy,0.0,1.0);
    }
  } else {
      discard;
  }

}

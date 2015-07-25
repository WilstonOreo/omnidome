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
uniform vec3 color;

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

void main()
{
  float d = frustum_intersection(vVertexPosition);  

  if (d > 0.0)
  {
    float alpha = d < 0.1 ? 0.5 : 0.1;
    gl_FragColor = vec4(color,alpha);
  } else
  {
    gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  }
}

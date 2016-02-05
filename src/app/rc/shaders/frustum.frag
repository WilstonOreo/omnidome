#version 120
 varying vec3 vVertexPosition;
 varying vec3 vVertexNormal;
uniform vec3 eye;
uniform vec3 look_at;
uniform vec3 top_left;
uniform vec3 top_right;
uniform vec3 bottom_left;
uniform vec3 bottom_right;
uniform int view_mode; // 0 = inside, 1 = outside, 2 = both

/// Color of frustum border
uniform vec3 color;

/// Tests if a point intersects frustum. Returns minimum distance from point to frustum
/// If distance > 0, there is an intersection
float frustum_intersection(in vec3 point)
{
  vec3 o = normalize(point - eye);
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

  float angle =  dot(vVertexNormal,normalize(vVertexPosition - eye));
  if (view_mode == 1) {
      angle = -angle;
  }
  if (view_mode == 2) {
      angle = 0.1;
  }

  float alpha = d < angle*0.1 ? 0.8*angle : 0.2*angle;

  if (d > 0.0 && angle > 0.0)
  {
    gl_FragColor = vec4(color,alpha);
  } else
  {
    gl_FragColor = vec4(0.0,0.0,0.0,0.0);
  }
}

#version 120
 varying vec3 vVertexPosition;
 varying vec3 vVertexNormal;

uniform float scale;
uniform float opacity;
uniform vec3 eye;
uniform vec3 look_at;
uniform vec3 top_left;
uniform vec3 top_right;
uniform vec3 bottom_left;
uniform vec3 bottom_right;
uniform float frame_width;
uniform int view_mode; // 0 = inside, 1 = outside, 2 = both

/// Color of frustum border
uniform vec3 color;

uniform mat4 proj_matrix;

uniform mat4 matrix;

uniform sampler2D image;
uniform float image_opacity;

float cross( in vec2 a, in vec2 b ) { return a.x*b.y - a.y*b.x; }

vec2 get2dPoint(vec3 p, mat4 viewMatrix,
                 mat4 projectionMatrix) {
      mat4 viewProjectionMatrix = projectionMatrix;
      //transform world to clipping coordinates
      vec3 pt = p * mat3(viewProjectionMatrix);
      vec2 p2d = (pt.xy + 1.0) / 2.0;
      p2d.y = 1.0 - p2d.y;

      return fract(p2d);
}

/// Project point to look at plane
vec2 projectPoint(vec3 p) {
  float dist = dot(p, look_at);
  vec3 proj = p - dist * look_at;
  return proj.xz / proj.y;
}

vec2 frustum_coordinates(in vec3 point) {
  vec3 p = point - eye;
  return get2dPoint(point,matrix,proj_matrix);
}


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
  if (d < 0.0) return -100000000.0;

  float d_top = dot(o,n_top);
  float d_bottom = dot(o,n_bottom);
  float d_left = dot(o,n_left);
  float d_right = dot(o,n_right);

  if ((d_top < 0.0) && (d_bottom < 0.0) && (d_left < 0.0) && (d_right < 0.0))
  {
    return -1000000.0; // min(min(-d_top,-d_bottom),min(-d_left,-d_right));
  }

  /// Return minimum distance
  return min(min(d_top,d_bottom),min(d_left,d_right));
}

void main()
{
  float d = frustum_intersection(vVertexPosition) * scale;

  float angle = dot(vVertexNormal,normalize(vVertexPosition - eye));
  if (view_mode == 1) {
      angle = -angle;
  }
  if (view_mode == 2) {
      angle = 0.1;
  }

  float alpha = d < frame_width ? 1.0 : 0.1;

  if (d > 0.0 && angle > 0.0)
  {
    vec2 texCoord = frustum_coordinates(vVertexPosition);
    vec3 c = mix(color,texture2D(image,texCoord).rgb,image_opacity);
  //  vec3 c = mix(color,vec3(texCoord,0.0),1.0);
    gl_FragColor = vec4(c,alpha * opacity);
  } else
  {
    discard;
  }
}

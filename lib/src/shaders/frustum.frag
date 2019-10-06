#version 120
 varying vec3 vVertexPosition;
 varying vec3 vVertexNormal;

uniform float opacity;
uniform vec3 eye;
uniform float frame_width;
uniform int view_mode; // 0 = inside, 1 = outside, 2 = both

/// Color of frustum border
uniform vec3 color;

uniform mat4 proj_matrix;

uniform sampler2D image;
uniform float image_opacity;

vec3 frustum_screencoords(in vec3 point) {
  vec4 p = proj_matrix * vec4(point,1.0);
  return vec3((p.xy / p.w + vec2(1.0)) * 0.5,p.z);
}

void main()
{
  float angle = dot(vVertexNormal,normalize(vVertexPosition - eye));
  // Outside view mode
  if (view_mode == 1) {
      angle = -angle;
  }
  // Both side view mode
  if (view_mode == 2) {
      angle = abs(angle);
  }

  vec3 uvw = frustum_screencoords(vVertexPosition);
  bool screencoordsInRange =
    (uvw.x >= 0.0) &&
    (uvw.y >= 0.0) &&
    (uvw.x < 1.0) &&
    (uvw.y < 1.0) && (uvw.z > 0.0);

  float alpha = min(min(uvw.x,1.0 - uvw.x),min(uvw.y,1.0 - uvw.y))  < frame_width ? 1.0 : 0.1;

  if (screencoordsInRange && angle > 0.0)
  {
    gl_FragColor = vec4(color,alpha * opacity);
  } else
  {
    discard;
  }
}

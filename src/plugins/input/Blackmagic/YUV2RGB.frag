#version 120

uniform vec2 texture_size;
uniform sampler2D texture;

varying vec2 texture_coords;

vec3 yuv2rgb(vec3 yuv) {
    float y = yuv.x, u = yuv.y - 0.5, v = yuv.z - 0.5;
    return vec3(
      y + 1.370705 * v,
      y - 0.698001 * v - 0.337633 * u,
      y + 1.732446 * u
    );
}

void main() {
  vec3 color;  
  vec2 uv = floor(texture_size * texture_coords);
    
  // Convert to YUV
  vec4 uyvy = texture2D(texture,uv / texture_size);
  
  if (mod(uv.x,2.0) == 0.0) {
    color = yuv2rgb(uyvy.yxz);
    } else {
    color = yuv2rgb(uyvy.wxz);
  }
//  gl_FragColor = vec4(texture_coords,0.0,1.0);

  gl_FragColor = vec4(color,1.0);
}

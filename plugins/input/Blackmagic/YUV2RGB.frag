#version 120

uniform vec2 texture_size;
uniform sampler2D texture;

varying vec2 texture_coords;

vec3 yuv2rgb(float y, vec2 uv) {
    uv -= vec2(0.5);
    vec4 uuvv = uv.yyxx * vec4(1.596,-0.813,-0.391,2.018);
    uuvv.z += uuvv.w;
    return vec3(1.164 * (y - 1.0 / 16.0)) + uuvv.xyz;
}

void main() {  
  vec4 uyvy = texture2D(texture,texture_coords);
  // Choose the right Y component
  float y = (mod(floor(texture_coords.x * texture_size.x),2.0) == 0.0) ? uyvy.y : uyvy.w;

  // Convert to YUV and output color
  gl_FragColor = vec4(yuv2rgb(y,uyvy.xz),1.0);
}

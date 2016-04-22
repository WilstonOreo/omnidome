
uniform sampler2D warp;
uniform sampler2D blend;

varying vec2 vTexCoord;

void main() {
  vec2 uv = vec2(vTexCoord.x,1.0 - vTexCoord.y);
  vec3 _warpColor = texture2D(warp,uv).rgb;
  float _blend = texture2D(blend,uv).r;

  gl_FragColor =
    (_warpColor == vec3(0.0,0.0,1.0)) ?
    vec4(0.0) : vec4(_warpColor,_blend);
}

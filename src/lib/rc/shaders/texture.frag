#version 120
varying vec2 vTexCoord;
uniform sampler2D tex;

void main() {
  //gl_FragColor = vec4(vTexCoord.xy,texture2D(tex,vTexCoord).r,1.0);
  gl_FragColor = texture2D(tex,vTexCoord);
}

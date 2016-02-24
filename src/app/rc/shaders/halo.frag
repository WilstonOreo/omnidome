
varying vec2 texCoord;

uniform vec4 color;

void main() {
  gl_FragColor = vec4(color.rgb,color.a * (1.0 - clamp(texCoord.y,0.0,1.0)));
}


varying vec2 vTexCoord;
uniform sampler2DRect texture;

void main() {
  gl_FragColor = texture2DRect(texture,vTexCoord);
}

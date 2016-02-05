uniform sampler2D a;
uniform sampler2D b;
uniform float factor;
uniform float additive;
uniform vec2 resolution;

void main()
{
  vec2 uv = gl_TexCoord[0].st;
  gl_FragColor = texture2D(b,(texture2D(a,uv).st+0.5)*0.5);
}


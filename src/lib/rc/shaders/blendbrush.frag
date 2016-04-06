#version 120

uniform sampler2D blend_tex;
//uniform vec2 blend_tex_size;

void main()
{
  vec2 texCoords = gl_TexCoord[0].st;
  gl_FragColor = vec4(0.0,0.0,0.0,texture2D(blend_tex,texCoords));
}

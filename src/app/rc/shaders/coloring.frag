#version 120
varying vec3 vVertexPosition;
varying vec3 vVertexNormal;

uniform sampler2D texture;
uniform vec3 color;

vec3 grayscale(vec3 c)
{
  return vec3(c*0.299,c*0.587,c*0.114);
}

void main()
{
  vec2 texCoords = gl_TexCoord[0].st;
 
  vec4 color = texture2D(texture,texCoords);

  gl_FragColor = vec4(color*grayscale(color.rgb),1.0);
}

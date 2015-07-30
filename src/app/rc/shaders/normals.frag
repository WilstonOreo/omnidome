#version 120
varying vec3 vVertexPosition;
varying vec3 vVertexNormal;

uniform sampler2D texture;
uniform bool lower_bits;

void main()
{
  vec2 texCoords = gl_MultiTexCoord0
  vec4 color = texture2D(texture,texCoords);
  
  gl_FragColor = vec4(gl_Normal*scale,1.0);
}

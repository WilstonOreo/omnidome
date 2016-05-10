#version 120

varying vec3 vVertexPosition;
varying vec3 vVertexNormal;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  vVertexPosition = (gl_ModelViewMatrix * gl_Vertex).xyz;
  vVertexNormal = gl_NormalMatrix * gl_Normal;
}

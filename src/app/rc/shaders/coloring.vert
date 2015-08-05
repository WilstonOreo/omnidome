#version 120

varying vec3 vVertexPosition;
varying vec3 vVertexNormal;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  vVertexPosition = (gl_Vertex * gl_ModelViewMatrix).xyz;
  vVertexNormal = gl_Normal * gl_NormalMatrix;
}


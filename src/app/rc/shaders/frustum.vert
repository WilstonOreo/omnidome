#version 120

varying vec3 vVertexPosition;
varying vec3 vVertexNormal;

uniform mat4 matrix;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  vVertexPosition = (gl_Vertex * matrix * gl_ModelViewMatrix).xyz; 
  vVertexNormal = gl_Normal * gl_NormalMatrix;
}


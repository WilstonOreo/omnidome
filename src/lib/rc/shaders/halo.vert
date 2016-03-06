#version 120

varying vec2 texCoord;

uniform mat4 matrix;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    texCoord = gl_TexCoord[0].st;
}

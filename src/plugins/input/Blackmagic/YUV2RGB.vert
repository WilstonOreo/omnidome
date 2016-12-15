#version 120

varying vec2 texture_coords;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

  texture_coords = gl_TexCoord[0].xy;
}

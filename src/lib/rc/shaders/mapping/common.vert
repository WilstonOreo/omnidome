#version 120

varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  uvw_vertex_position = (gl_Vertex * gl_TextureMatrix[0]).xyz;
  uvw_normal = normalize(gl_Normal.xyz);
}

#version 120

varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;
varying vec3 v_pos;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  uvw_vertex_position = (gl_Vertex * gl_ModelViewMatrix).xyz;
  v_pos = vec3(gl_ModelViewMatrix * gl_Vertex);

  uvw_normal = normalize((gl_NormalMatrix * gl_Normal).xyz);
}

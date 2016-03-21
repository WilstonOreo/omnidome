#version 120

varying vec3 uvw_vertex_position;
varying vec3 uvw_normal;

uniform bool bound_to_canvas;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	if (bound_to_canvas) {
  		uvw_vertex_position = (gl_Vertex).xyz;
	} else {
  		uvw_vertex_position = (gl_Vertex * gl_ModelViewMatrix).xyz;
	}

  	uvw_normal = normalize((gl_NormalMatrix * gl_Normal).xyz);
}

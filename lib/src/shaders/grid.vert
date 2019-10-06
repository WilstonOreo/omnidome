#version 120 

varying vec2 texture_coordinate;
varying vec3 eye_vec;
varying vec3 v;

void main() 
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  texture_coordinate = vec2(gl_MultiTexCoord0.xy);

  v  = gl_Vertex.xyz; 
}

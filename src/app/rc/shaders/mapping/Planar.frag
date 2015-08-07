
float mapping(in vec3 uvw, out vec2 texCoords) 
{
  texCoords = gl_TexCoord[0].st;
  return 1.0;
}

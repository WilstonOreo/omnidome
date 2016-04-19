
uniform vec2 stretch;

float mapping(in vec3 uvw, out vec2 texCoords)
{
  texCoords = offset.xy + stretch * gl_TexCoord[0].st;
  if (fract(texCoords) != texCoords) return -1.0;
  return 1.0;
}

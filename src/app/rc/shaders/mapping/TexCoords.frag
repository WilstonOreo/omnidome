
uniform vec2 offset;
uniform vec2 stretch;

float intersection(out vec3 uvw) {
    return 1.0;
}

float mapping(in vec3 uvw, out vec2 texCoords)
{
  texCoords = offset + stretch * gl_TexCoord[0].st;
  if (fract(texCoords) != texCoords) return -1.0;
  return 1.0;
}

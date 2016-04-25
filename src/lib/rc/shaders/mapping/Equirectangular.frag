
uniform float strip_top; // 0.0
uniform float strip_bottom; // 1.0

float mapping(in vec3 uvw, out vec2 texCoords)
{
  vec3 norm = normalize(uvw);
  texCoords.s = fract(atan(norm.y,norm.x) / (2.0*PI));
  texCoords.t = 1.0 - acos(norm.z) / PI;

  if (texCoords.t < strip_bottom || texCoords.t > strip_top)
  {
    return -1.0;
  }
  texCoords.t = fract(( texCoords.t - strip_bottom) / (strip_top - strip_bottom));

  return 1.0;
}

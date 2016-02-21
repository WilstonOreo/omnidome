
uniform float strip_top; // 0.0
uniform float strip_bottom; // 1.0

uniform vec3 offset;

float intersection(out vec3 uvw) {
    return sphereIntersection(uvw,offset);

}

float mapping(in vec3 uvw, out vec2 texCoords)
{
  texCoords.s = fract(atan(uvw.y,uvw.x) / (2.0*PI));
  texCoords.t = 1.0 - acos(uvw.z) / PI;

  if (texCoords.y < strip_bottom || texCoords.y > strip_top)
  {
    return -1.0;
  }
  texCoords.t = fract(( texCoords.y - strip_bottom) / (strip_top - strip_bottom));

  return 1.0;
}

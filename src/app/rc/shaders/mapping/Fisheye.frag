
uniform float stretch;
uniform vec3 offset;

float intersection(out vec3 uvw) {
    return sphereIntersection(uvw,offset);
}

float mapping(in vec3 uvw, out vec2 texCoords)
{
  float phi = atan(sqrt(uvw.x*uvw.x + uvw.y*uvw.y),uvw.z);
  float r =  phi / PI * 2.0 / (stretch + 1.0);
  if ((r > 1.0) || (r <= 0.0)) return -1.0;
  float theta = atan(uvw.x,uvw.y);
  texCoords.s = fract(0.5 * (1.0 + r* sin(theta)));
  texCoords.t = fract(0.5 * (1.0 + r * cos(theta)));
  return 1.0;
}

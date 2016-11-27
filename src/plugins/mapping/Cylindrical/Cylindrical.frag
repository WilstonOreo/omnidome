
float mapping(in vec3 uvw, out vec2 texCoords)
{
  texCoords.s = fract(atan(uvw.y,uvw.x) / (2.0*PI));
  texCoords.t = fract(uvw.z);
  return 1.0;
}

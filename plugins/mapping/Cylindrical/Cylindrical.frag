
uniform float angle_begin;
uniform float angle_end;

float deg2rad(in float deg) {
  return deg * PI / 180.0;
}

float mapping(in vec3 uvw, out vec2 texCoords)
{
  float theta =  fract(atan(uvw.y,uvw.x) / (2.0*PI));

  if (theta < angle_begin || theta > angle_end) {
      return -1.0;
  }

  texCoords.s = (theta - angle_begin) / (angle_end - angle_begin);
  texCoords.t = fract(uvw.z);
  return 1.0;
}


uniform float strip_top; // 0.0
uniform float strip_bottom; // 1.0

uniform vec3 offset;

float intersection(out vec3 uvw) {
    vec3 n = uvw_normal;
    vec3 oc = normalize(uvw_vertex_position - offset);
    float b = 2.0 * dot(n, oc);
    float c = dot(oc, oc) - 1.0;
    float disc = b * b - 4.0 * c;

    if (disc < 0.0)
        return -1.0;

    // compute q as described above
    float q;
    if (b < 0.0)
        q = (-b - sqrt(disc))/2.0;
    else
        q = (-b + sqrt(disc))/2.0;

    float t0 = q;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1) {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < -0.0001)
        return -1.0;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < -0.001) {
        uvw = oc + t1 * n;
    } else {
        uvw = oc + t0 * n;
    }

    return 1.0;
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

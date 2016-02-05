
uniform float stretch;
uniform vec3 offset;

float intersection(out vec3 uvw) {
    vec3 oc = uvw_vertex_position - offset;
    float b = 2.0 * dot(uvw_normal, oc);
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
    if (t1 < 0.0)
        return -1.0;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0.0) {
        uvw = oc + t1 * uvw_normal;
    } else {
        uvw = oc + t0 * uvw_normal;
    }

    return 1.0;
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

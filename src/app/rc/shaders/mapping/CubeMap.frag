
uniform vec3 offset;

void swap(inout float t0, inout float t1) {
    float t = t0;
    t0 = t1;
    t1 = t;
}

float intersection(out vec3 uvw) {
    vec3 dir = uvw_normal;
    vec3 orig = uvw - offset;
  float tmin = (-0.5 - orig.x) / dir.x;
   float tmax = (0.5 - orig.x) / dir.x;

   if (tmin > tmax) {
       swap(tmin, tmax);
   }

   float tymin = (-0.5 - orig.y) / dir.y;
   float tymax = ( 0.5 - orig.y) / dir.y;

   if (tymin > tymax) swap(tymin, tymax);

   if ((tmin > tymax) || (tymin > tmax)) {
       return -1.0;
   }

   if (tymin > tmin) {
       tmin = tymin;
   }

   if (tymax < tmax) {
       tmax = tymax;
   }

   float tzmin = ( 0.5 - orig.z) / dir.z;
   float tzmax = (-0.5 - orig.z) / dir.z;

   if (tzmin > tzmax) {
       swap(tzmin, tzmax);
   }

   if ((tmin > tzmax) || (tzmin > tmax)) {
       return -1.0;
   }

   if (tzmin > tmin) {
       tmin = tzmin;
   }

   if (tzmax < tmax) {
       tmax = tzmax;
   }

   uvw = uvw_vertex_position + tmin * dir;

   return 1.0;
}



float mapping(in vec3 uvw, out vec2 texCoords)
{
  float sc, tc, ma;
  float eps =  -0.01;
  float _off = 0.0;
  //n = n.yzx;

  if ((abs(uvw.x) >= abs(uvw.y)) && (abs(uvw.x) >= abs(uvw.z)))
  {
    sc = uvw.y;
    tc = uvw.z;
    if (uvw.x < 0.0) { tc = -uvw.z; }
    ma = uvw.x;
    _off += (uvw.x > 0.0) ? 3.0/6.0 : 2.0/6.0; // EAST / WEST
  } else
  if ((abs(uvw.y) >= abs(uvw.z)))
  {
    sc = uvw.z;
    tc = uvw.x;
    if (uvw.y < 0.0) { sc = -uvw.z; }
    ma = uvw.y;
    _off += (uvw.y > 0.0) ? 1.0/6.0 : 0.0/6.0; // NORTH / SOUTH
  } else
  {
    sc = (uvw.z > 0.0) ? uvw.y : -uvw.y;
    tc = (uvw.z > 0.0) ? -uvw.x : -uvw.x;
    ma = uvw.z;
    _off = (uvw.z > 0.0) ? 4.0/6.0 : 5.0 / 6.0;  // TOP / BOTTOM
  }
  texCoords = vec2(sc/(12.0 - eps)/abs(ma) + 0.5/6.0 + _off,0.5+tc/(2.0 - eps)/abs(ma)) ;
  return 1.0;
}

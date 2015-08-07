
float mapping(in vec3 uvw, out vec2 texCoords) 
{
  float sc, tc, ma;
  float eps =  -0.02;
  float _off = 0.0;
  //n = n.yzx;

  if ((abs(uvw.x) >= abs(uvw.y)) && (abs(uvw.x) >= abs(uvw.z)))
  {
    sc = (uvw.x > 0.0) ? -uvw.y : uvw.y;
    tc = uvw.z;
    ma = uvw.x;
    _off += (uvw.x < 0.0) ? 0.0/6.0 : 2.0/6.0; // EAST / WEST
  } else
  if ((abs(uvw.y) >= abs(uvw.z)))
  {
    sc = (uvw.y < 0.0) ? -uvw.x : uvw.x;
    tc = uvw.z;
    ma = uvw.y;
    _off += (uvw.y < 0.0) ? 1.0/6.0 : 3.0/6.0; // NORTH / SOUTH
  } else
  {
    sc = (uvw.z > 0.0) ? uvw.y : uvw.y;
    tc = (uvw.z > 0.0) ? uvw.x : -uvw.x;
//    tc = -n.x; 
    ma = uvw.z;
    _off = (uvw.z < 0.0) ? 4.0/6.0 : 5.0 / 6.0;  // TOP / BOTTOM
  }
  texCoords = vec2(sc/(12.0 - eps)/abs(ma) + 0.5/6.0 + _off,0.5+tc/(2.0 - eps)/abs(ma)) ;
  return 1.0;
}


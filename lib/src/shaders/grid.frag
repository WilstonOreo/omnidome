#version 120

uniform vec2 mouse;
uniform vec2 resolution;

varying vec2 texture_coordinate;
varying vec3 v;

uniform vec3 cam_pos;
uniform vec3 dir;
uniform float alpha;

float dist;

float onGridline(float _pos, float num)
{
  return clamp(1.0 - (dist / num)*sin(mod(_pos,1.0 / num)*num*3.14159),0.0,1.0);
}

float onGridline(vec2 _pos, vec2 num)
{
  return clamp(
    onGridline(_pos.x,num.x) +
    onGridline(_pos.y,num.y),0.0,1.0);
}


void main(void)
{
  vec2 pos = v.xy / vec2(1000.0,1000.0);

  dist = length(resolution) / length(cam_pos - v) * 200.0;

  float numGrid = 1.0;
  float grid = 0.0;

  float numInc = 10.0;
  int i = 0;

  float stopValue = dist * sin(abs(normalize(dir).z)*3.1415 * 0.5);

  while (numGrid < stopValue)
  {
    float numLines = (mod(i,2) == 0.0) ? 2.0 : 5.0;

    float inc = onGridline(pos,vec2(numGrid)) / log(numGrid*numLines) * log(numLines);
    numGrid *= numLines;
    if (numGrid >= stopValue)
      inc *=  1.0 - fract((numGrid/numLines - stopValue) / numGrid);

    grid += inc;
    ++i;
  }

  grid = clamp(grid,0.0,0.5);
  grid *= 2.0;

  float viewAlpha = alpha;
  if ((dir.z < 0.0) && (cam_pos.z - v.z < 0.0) ) viewAlpha *= 0.25;

  gl_FragColor = vec4(1.0,1.0,1.0,viewAlpha*grid);
}

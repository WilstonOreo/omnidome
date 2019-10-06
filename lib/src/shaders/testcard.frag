#version 120

uniform float time;
varying vec2 vTexCoord;
uniform vec2 resolution;
uniform vec3 test_color;
uniform int projector_id;
uniform bool gray_output;
const float soft = 0.002;

const float PI = 3.14159265358979323846264;

#include ":/shaders/grayscale.h"

float circle(vec2 pos, float r)
{
float _distance = length(pos);
float _v = (_distance ) / r;
return 1.0 - smoothstep(1.0 - soft / r,1.0,_v);
}

float checkboard(vec2 pos)
{
float id = 0.5 + 0.5*cos(time + sin(dot(floor(pos+0.5),vec2(113.1,17.81)))*43758.545);
vec2  pa = smoothstep( 0.0, 0.2, id*(0.5 + 0.5*cos(6.2831*pos)) );
return pa.x * pa.y;
}


float grid(vec2 pos)
{
float width = soft * 25.0;
pos += width * 0.5;

float id = 0.5 + 0.5*cos(time + sin(dot(floor(pos+0.5),vec2(113.1,17.81)))*43758.545);
return clamp(4.0 *
    (min(smoothstep(0.0,width, mod(pos.x + 0.5,1.0)),
    smoothstep(1.0 - width,1.0, mod(1.0 - pos.x - 0.5,1.0))) +
    min(smoothstep(0.0,width, mod(pos.y + 0.5,1.0)),
    smoothstep(1.0 - width,1.0, mod(1.0 - pos.y - 0.5,1.0)))),0.0,1.0);
}

vec3 palette(vec2 pos)
{
int index = int(clamp(pos.x*8.0,0.0,8.0));

 if (index == 0) return vec3(0.5,0.5,0.5);
 if (index == 1) return vec3(1.0,1.0,0.0);
 if (index == 2) return vec3(0.0,1.0,1.0);
 if (index == 3) return vec3(0.0,1.0,0.0);
 if (index == 4) return vec3(1.0,0.0,1.0);
 if (index == 5) return vec3(1.0,0.0,0.0);
 if (index == 6) return vec3(0.0,0.0,1.0);
 if (index == 7) return vec3(0.0,0.0,0.0);

return vec3(0.0);
}

vec3 grayscale(vec2 pos, float steps)
{
return vec3(0.3 + int(pos.x * steps) / steps * float(0.7));
}

vec3 interference(vec2 pos, float steps)
{
float freq = pow((int(pos.x * steps) / steps),2.0) * 100.0 * steps * PI;

return vec3(0.5)*(1.0 + cos(pos.x * freq));
}

vec3 hueGradient(float t) {
  vec3 p = abs(fract(t + vec3(1.0, 2.0 / 3.0, 1.0 / 3.0)) * 6.0 - 3.0);
return (clamp(p - 1.0, 0.0, 1.0));
}

vec3 complementGradient(float t, vec3 color)
{
vec3 p = t <= 0.5 ?
  mix( vec3(0.0),color,t) :
  mix( color,1.0 - color,t);
return p;
}

vec3 complementGradient(vec2 pos)
{
  float oneThird = 1.0 / 3.0;

  if (pos.y <= oneThird) return complementGradient(pos.x,vec3(1.0,0.0,0.0));
  if (pos.y <= 2.0 * oneThird) return complementGradient(pos.x,vec3(0.0,1.0,0.0));
  if (pos.y <= 1.0) return complementGradient(pos.x,vec3(0.0,0.0,1.0));

  return vec3(1.0,1.0,1.0);
}

vec3 testcard(vec2 pos)
{
vec2 gridPos = pos* 8.0 - 0.5;
vec3 g = vec3(grid(gridPos)) + test_color * checkboard(gridPos);

if (abs(gridPos.x + 0.5) <= 5.99)
{
  float x = (gridPos.x + 0.5) / 12.0 + 0.5;
  float y = gridPos.y + 0.5;
  vec2 v = vec2(x,y);
  if (y <= 4.99 && y >= 2.01)
    g = palette(v);
  if (y <= 1.99 && y >= 0.01)
    g = grayscale(v,5.0);

  if (y <= -1.01 && y >= -1.99)
    g = interference(v,10.0);
  if (y <= -2.01 && y >= -2.99)
    g = hueGradient(v.x);
  if (y <= -3.01 && y >= -4.99) // Red complement
    g = complementGradient((v + vec2(0.0,5.0))/vec2(1.0,2.0));
  }

g += clamp(circle(pos,10.0 / 12.0) - circle(pos,10.0 / 12.0 - 0.005),0.0,1.0);

if (gray_output) {
    g = grayscale(g);
}

return g;
}


void main(void)
{
vec2 uv = vTexCoord;
vec2 pos = (uv - 0.5) * 2.0 * (resolution.xy / resolution.y);// (-resolution.xy + 2.0*uv)/resolution.y;

gl_FragColor = vec4(testcard(pos), 1.0 );
}


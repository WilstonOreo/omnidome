varying vec2 vTexCoord;

uniform float width;
uniform float soft;

float circle(vec2 pos, float r)
{
  float _distance = length(pos);
  float _v = (_distance ) / r;
  return 1.0 - smoothstep(1.0 - soft / r,1.0,_v);
}

void main( )
{
	  vec2 uv = vTexCoord * 2.0 - 1.0;
    float outer_radius = 1.0 - soft;
    float v = circle(uv,outer_radius) - circle(uv,outer_radius - width);
    float inner = clamp(circle(uv,outer_radius) -circle(uv,outer_radius - width*2.0),0.0,1.0);
    gl_FragColor = vec4(v,v,v,inner);
}

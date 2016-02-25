
uniform float top;
uniform float bottom;
uniform float left;
uniform float right;
uniform float mask;
uniform float gamma;
uniform vec3 color;
uniform sampler2D input_tex;
uniform float input_opacity;

float edgeblend_border(in vec2 coord)
{
  float edgeValue = 1.0;

  if (coord.x <= left)
  {
    edgeValue *= min(coord.x / left, 1.0);
  } else
  if (coord.x >= 1.0 - right)
  {
    edgeValue *= min((1.0 - coord.x) / right, 1.0);
  }
  if (1.0 - coord.y <= top)
  {
    edgeValue *= min((1.0 - coord.y) / top, 1.0);
  }
  if (coord.y <= bottom)
  {
    edgeValue *= min(coord.y / bottom, 1.0);
  }

  return 1.0 - pow(clamp(edgeValue,0.0,1.0),gamma);
}

vec4 edgeblend_color(in float value)
{
  float v = clamp(value,0.0,1.0);
  if (mask < 0.0)
  {
  	return vec4(0.0,0.0,0.0,v);
  }
  float mv = 1.0 - v;
  return vec4(mv,mv,mv,1.0);
}

void main()
{
  vec2 texCoords = gl_TexCoord[0].st;

  gl_FragColor = mix(
      vec4(color,1.0),
    vec4(texture2D(input_tex,texCoords).rgb,1.0),input_opacity)*edgeblend_color(edgeblend_border(texCoords));
}

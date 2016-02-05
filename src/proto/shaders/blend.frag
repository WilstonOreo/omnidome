uniform sampler2D tex_a;
uniform sampler2D tex_b;
uniform float factor;

// 0 = crossfade, 1 = additive, 2 = slide_blend, 3 = move_blend
uniform float blend_mode;

vec4 crossfade(in vec2 pos, in vec4 a, in vec4 b, in float blend)
{
  return mix(a,b,blend);
}

vec4 additiveBlend(in vec2 pos, in vec4 a, in vec4 b, in float blend)
{
  return mix(a,b,blend) * (1.0 + pow(sin(blend*3.1415),2.0)*vec4(1.0));
}

vec4 slideBlend(in vec2 pos, in vec4 a, in vec4 b, in float blend)
{
  return (pos.x < blend) ? b : a;
}

vec4 moveBlend(in vec2 pos, in float blend)
{
  vec2 shift = vec2(blend,0.0);
  return (pos.x < blend) ? texture2D(tex_b,pos - shift) : texture2D(tex_a,pos - shift);
}

void main()
{
  vec2 uv = gl_TexCoord[0].st;
  int mode = int(blend_mode);
  
  vec4 a = texture2D(tex_a,uv);
  vec4 b = texture2D(tex_b,uv);
  float f = clamp(factor,0.0,1.0);

  if (blend_mode == 0)
  {
    gl_FragColor = crossfade(uv,a,b,f);
  } else
  if (blend_mode == 1)
  {
    gl_FragColor = additiveBlend(uv,a,b,f);
  } else
  if (blend_mode == 2)
  {
    gl_FragColor = slideBlend(uv,a,b,f);
  } else
  if (blend_mode == 3)
  {
    gl_FragColor = moveBlend(uv,f);
  }
}


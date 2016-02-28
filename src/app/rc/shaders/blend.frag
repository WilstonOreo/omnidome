#version 120

uniform float top;
uniform float bottom;
uniform float left;
uniform float right;
uniform float mask;
uniform float edge_gamma;
uniform vec3 color;
uniform sampler2D input_tex;
uniform float input_opacity;

/// Color Correction parameters
uniform vec4 cc_gamma;
uniform vec4 cc_brightness;
uniform vec4 cc_contrast;
uniform vec4 cc_multiplier;

struct ChannelCorrection {
    float gamma;
    float brightness;
    float contrast;
    float multiplier;
};

ChannelCorrection
    cc_red = ChannelCorrection(cc_gamma.r, cc_brightness.r, cc_contrast.r, cc_multiplier.r),
    cc_green = ChannelCorrection(cc_gamma.g, cc_brightness.g, cc_contrast.g, cc_multiplier.g),
    cc_blue = ChannelCorrection(cc_gamma.b, cc_brightness.b, cc_contrast.b, cc_multiplier.b),
    cc_all = ChannelCorrection(cc_gamma.a, cc_brightness.a, cc_contrast.a, cc_multiplier.a);

float brightness(float v, float brightness_value) {
    return max(v + brightness_value,0.0);
}

/// Calculate contrast corrected value
float contrast(float v, float contrast_value) {
    float _c = (contrast_value <= 0.0) ?
        contrast_value + 1.0 :
        (1.0 / (1.0 - contrast_value));
    return max((v - 0.5) * max(_c, 0.0f) + 0.5,0.0);
}

/// Calculate gamma corrected value
float gamma(float v, float gamma_value) {
    float g = 0.0;
    if (gamma_value >= 0.0) {
        g = 1.0 / (1.0 + gamma_value);
    } else {
        g = (1.0 - gamma_value);
    }
    return pow(v,g*g*g*g);
}

float corrected(in ChannelCorrection c, in float v) {
    return gamma(contrast(brightness(v,
        c.brightness * c.multiplier),
        c.contrast * c.multiplier),
        c.gamma * c.multiplier);
}


vec3 color_correction(vec3 color) {
    if (cc_all.multiplier == 0.0) {
        return color;
    }
    return clamp(vec3(
        corrected(cc_all,corrected(cc_red,color.r)),
        corrected(cc_all,corrected(cc_green,color.g)),
        corrected(cc_all,corrected(cc_blue,color.b))
    ),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0));
}


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

  return 1.0 - pow(clamp(edgeValue,0.0,1.0),edge_gamma);
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
    vec4(color_correction(texture2D(input_tex,texCoords).rgb),1.0),input_opacity)*edgeblend_color(edgeblend_border(texCoords));
}

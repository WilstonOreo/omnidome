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
uniform bool gray_output;

/// Color Correction parameters
uniform vec4 cc_gamma;
uniform vec4 cc_brightness;
uniform vec4 cc_contrast;
uniform vec4 cc_multiplier;

#include ":/shaders/colorcorrection.h"
#include ":/shaders/grayscale.h"

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
  float mv = 1.0 - v * mask;
  return vec4(mv,mv,mv,1.0);
}


void main()
{
  cc_red = ChannelCorrection(cc_gamma.r, cc_brightness.r, cc_contrast.r, cc_multiplier.r);
  cc_green = ChannelCorrection(cc_gamma.g, cc_brightness.g, cc_contrast.g, cc_multiplier.g);
  cc_blue = ChannelCorrection(cc_gamma.b, cc_brightness.b, cc_contrast.b, cc_multiplier.b);
  cc_all = ChannelCorrection(cc_gamma.a, cc_brightness.a, cc_contrast.a, cc_multiplier.a);
  vec2 texCoords = gl_TexCoord[0].st;

  vec3 output_color = mix(
    color,
    color_correction(texture2D(input_tex,texCoords).rgb),
    input_opacity);

  if (gray_output) {
      output_color = grayscale(output_color);
  }

  gl_FragColor = vec4(output_color,1.0)*edgeblend_color(edgeblend_border(texCoords));
}

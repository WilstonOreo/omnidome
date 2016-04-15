/******************************************************************
 This file is part of Omnidome.

 Omnidome is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 DomeSimulator is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with DomeSimulator.  If not, see <http://www.gnu.org/licenses/>.

 Omnidome is free for non-commercial use. If you want to use it
 commercially, you should contact the author
 Michael Winkelmann aka Wilston Oreo by mail:
 me@wilstonoreo.net
 **************************************************************************/

////////////////////////////////////////////
// Color correction
////////////////////////////////////////////

struct ChannelCorrection {
    float gamma;
    float brightness;
    float contrast;
    float multiplier;
};

varying vec2 vTexCoord;

uniform vec4 cc_red_vec;
uniform vec4 cc_green_vec;
uniform vec4 cc_blue_vec;
uniform vec4 cc_all_vec;

ChannelCorrection cc_red;
ChannelCorrection cc_green;
ChannelCorrection cc_blue;
ChannelCorrection cc_all;

uniform bool map_mirror_vertical;
uniform bool map_mirror_horizontal;

uniform sampler2DRect image;
uniform vec2 image_size;
uniform sampler2D uv_map;

float brightness(float v, float brightness_value) {
    return max(v + brightness_value,0.0);
}

/// Calculate contrast corrected value
float contrast(float v, float contrast_value) {
    float _c = (contrast_value <= 0.0) ?
        contrast_value + 1.0 :
        (1.0 / (1.0 - contrast_value));
    return (v - 0.5) * max(_c, 0.0) + 0.5;
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
    return brightness(contrast(gamma(v,
        c.gamma * c.multiplier),
        c.contrast * c.multiplier),
        c.brightness * c.multiplier);
}

vec3 colorCorrection(vec3 color) {
    if (cc_all.multiplier == 0.0) {
        return color;
    }
    return clamp(vec3(
        corrected(cc_all,corrected(cc_red,color.r)),
        corrected(cc_all,corrected(cc_green,color.g)),
        corrected(cc_all,corrected(cc_blue,color.b))
    ),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0));
}

////////////////////////////////////////////
// END Color correction
////////////////////////////////////////////


void main()
{
    vec2 uv = vTexCoord;
    vec3 calib = texture2D(uv_map,uv).rga;

    cc_red = ChannelCorrection(cc_red_vec.x,cc_red_vec.y,cc_red_vec.z,cc_red_vec.w);
    cc_green = ChannelCorrection(cc_green_vec.x,cc_green_vec.y,cc_green_vec.z,cc_green_vec.w);
    cc_blue = ChannelCorrection(cc_blue_vec.x,cc_blue_vec.y,cc_blue_vec.z,cc_blue_vec.w);
    cc_all = ChannelCorrection(cc_all_vec.x,cc_all_vec.y,cc_all_vec.z,cc_all_vec.w);

    if (map_mirror_horizontal)
    {
        calib.s = 1.0 - calib.s;
    }
    if (map_mirror_vertical)
    {
        calib.t = 1.0 - calib.t;
    }

    vec3 imagePixel = colorCorrection(texture2DRect(image,calib.st * image_size).rgb);
    float alpha =  calib.b;
    gl_FragColor = vec4(imagePixel*alpha,1.0);
}

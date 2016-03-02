/*{
 "DESCRIPTION": "Omnidome TexCoords Map Shader",
 "CREDIT": "cr8tr.org",
 "CATEGORIES":
 [
 "Omnidome"
 ],
 "INPUTS":
 [
 {
 "NAME": "image",
 "LABEL": "Image",
 "TYPE": "image"
 },
 {
 "NAME": "texcoord_map",
 "LABEL": "TexCoord Map",
 "TYPE": "image"
 },
 {
 "NAME": "map_mirror_horizontal",
 "LABEL": "Mirror horizontal",
 "TYPE": "bool",
 "DEFAULT": 0
 },
 {
 "NAME": "map_mirror_vertical",
 "LABEL": "Mirror vertical",
 "TYPE": "bool",
 "DEFAULT": 0
 }
 ]
 }
 */
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

ChannelCorrection cc_red, cc_green, cc_blue, cc_all;

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

void getChannelCorrectionFromImage(
    in sampler2DRect image,
    in vec2 s,
    in vec2 uv,
    out ChannelCorrection c) {
    vec2 _shift = vec2(0.0,1.0 / 16.0 * s.y);
    c.gamma = (texture2DRect(image,uv).b - 0.5)*2.0;
    c.brightness = (texture2DRect(image,uv + 1.0*_shift).b - 0.5)*2.0;
    c.contrast = (texture2DRect(image,uv + 2.0*_shift).b - 0.5)*2.0;
    c.multiplier = (texture2DRect(image,uv + 3.0*_shift).b - 0.5)*2.0;
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
    gl_FragColor = vec4(0.0,0.0,0.0,1.0);

    vec2 s = _texcoord_map_imgSize;
    vec2 uv = vv_FragNormCoord + vec2(0.0,1.0/_texcoord_map_imgSize.y);

    vec2 uvw_upper = vec2(uv.x*s.x,(uv.y*s.y + s.y)/2.0);
    vec2 uvw_lower = vec2(uv.x*s.x,(uv.y*s.y)/2.0);

    vec2 texCoords = texture2DRect(texcoord_map,uvw_upper).st*2.0 -
        1.0 + texture2DRect(texcoord_map,uvw_lower).st*2.0/255.0 - 1.0 / 255.0;

    if (map_mirror_horizontal)
    {
        texCoords.s = 1.0 - texCoords.s;
    }
    if (map_mirror_vertical)
    {
        texCoords.t = 1.0 - texCoords.t;
    }

    vec2 _shift = vec2(0.0,s.y/2.0/4.0);
    getChannelCorrectionFromImage(texcoord_map,s,uvw_lower + 0.0*_shift,cc_all);
    getChannelCorrectionFromImage(texcoord_map,s,uvw_lower + 1.0*_shift,cc_red);
    getChannelCorrectionFromImage(texcoord_map,s,uvw_lower + 2.0*_shift,cc_green);
    getChannelCorrectionFromImage(texcoord_map,s,uvw_lower + 3.0*_shift,cc_blue);

    float alpha =  texture2DRect(texcoord_map,uvw_upper).b;
    gl_FragColor = vec4(texture2DRect(image, texCoords * _image_imgSize).rgb*alpha,1.0);
}

#version 120

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

uniform sampler2DRect image;
uniform sampler2D uv_map;
uniform vec2 image_size;

varying vec2 vTexCoord;

uniform vec4 cc_red_vec;
uniform vec4 cc_green_vec;
uniform vec4 cc_blue_vec;
uniform vec4 cc_all_vec;


uniform bool map_mirror_vertical;
uniform bool map_mirror_horizontal;

#include ":/shaders/colorcorrection.h"


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

    vec3 imagePixel = color_correction(texture2DRect(image,calib.rg * image_size).rgb);
    float alpha =  calib.b;

    gl_FragColor = vec4(imagePixel*alpha,1.0);
}

/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

struct ChannelCorrection {
    float gamma;
    float brightness;
    float contrast;
    float multiplier;
};

ChannelCorrection
cc_red,
cc_green,
cc_blue,
cc_all;

float brightness(float v, float brightness_value) {
    return max(v + brightness_value,0.0);
}

/// Calculate contrast corrected value
float contrast(float v, float contrast_value) {
    float _c = (contrast_value <= 0.0) ?
        contrast_value + 1.0 :
        (1.0 / (1.0 - contrast_value));
    return (v - 0.5) * max(_c, 0.0f) + 0.5;
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

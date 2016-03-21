/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#ifndef OMNIC_CALIBRATION_H_
#define OMNIC_CALIBRATION_H_

enum omnic_error_code {
    OMNIC_OK = 0,
    OMNIC_ERROR_READ = 1,
    OMNIC_ERROR_WRITE = 2
};

typedef struct omnic_pixel {
    float r,g,b,a;
};

typedef struct omnic_channelcorrection {
    float gamma, brightness, contrast, multiplier;
};

typedef struct omnic_colorcorrection {
    omnic_colorcorrection red;
    omnic_colorcorrection green;
    omnic_colorcorrection blue;
    omnic_colorcorrection all;
};

typedef struct omnic_calibrated_projector {
    uint32_t screen_info; // First bit = virtual screen
    int32_t screen_offset_x;
    int32_t screen_offset_y;
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t buffer_width;
    uint32_t buffer_height;
    omnic_colorcorrection colorcorrection;
    omni_pixel* buffer;
};

typedef struct omnic_calibration {
    uint32_t header_size;
    char* header;
    uint32_t num_projectors;
    int content_type; // 0 = TEXCOORDS, 1 = UVW_MAP
    omnic_calibrated_projector* projectors;
};

/// Read calibration from file
omnic_error_code omnic_read(FILE*,omni_calibration*);

/// Read calibration from file with name
omnic_error_code omnic_read(const char*,omni_calibration*);

/// Write calibration to file
omnic_error_code omnic_write(FILE*,omni_calibration*);

/// Write calibration to file with name
omnic_error_code omnic_write(const char*,omni_calibration*);

void omnic_free(omni_calibration*);

#endif /* OMNIC_CALIBRATION_H_ */

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

/// Error codes
enum omnic_error_code {
    OMNIC_OK = 0,
    OMNIC_ERROR_READ = 1,
    OMNIC_ERROR_WRITE = 2
};

/// Calibrated pixel as UVW texture coordinates
typedef struct omnic_uvw_pixel {
    float u,v,w; // Texture coordinates
    float b; // Blend mask value
};

/// Correction for a single color channel
typedef struct omnic_channelcorrection {
    /// Gamma correction from -1.0 to 1.0
    float gamma;

    /// Brightness correction from -1.0 to 1.0
    float brightness;

    /// Contrast correction from -1.0 to 1.0
    float contrast;

    /// Correction multiplier from 0.0 to 1.0
    float multiplier;
};

/// Color correction for red, green and blue channel
typedef struct omnic_colorcorrection {
    /// Color correction for red channel
    omnic_colorcorrection red;

    /// Color correction for green channel
    omnic_colorcorrection green;

    /// Color correction for blue channel
    omnic_colorcorrection blue;

    /// Overall color correction for all three channels
    omnic_colorcorrection all;
};

/// Rectangle struct
typedef struct omnic_rect {
    /// Offset in X direction
    int32_t offset_x;

    /// Offset in Y direction
    int32_t offset_y;

    /// Width of rectangle
    uint32_t width;

    /// Height of rectangle
    uint32_t height;
};


/**@brief A calibration for a single projector
 **/
typedef struct omnic_calibrated_projector {
    /// Screen info flags. First bit: virtual screen
    uint32_t screen_info;

    /// Rectangle representing the screen geometry
    omnic_rect screen_geometry;

    /// Rectangle representing the content placement inside the screen
    omnic_rect content_geometry;

    /// Three channel color correction
    omnic_colorcorrection colorcorrection;

    /// Buffer width
    uint32_t buffer_width;

    /// Buffer Height
    uint32_t buffer_height;

    /// Buffer with calibrated pixels
    omni_uvw_pixel* buffer;

    /// Brightness correction buffer width
    uint32_t brightness_correction_width;

    /// Brightness correction buffer height
    uint32_t brightness_correction_height;

    /// Buffer with brightness corrected pixels
    omnic_channelcorrection* brightness_correction;
};

/// Calibration for several projectors
typedef struct omnic_calibration {
    /// Header string
    char header[80];

    /**@brief Calibration content type
     **@detail 0 = UV coords, 1 = UVW coords
     **/
    uint32_t content_type;

    /// Number of projectors
    uint32_t num_projectors;

    /// List of calibrated projectors
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

/// Free memory of calibration
void omnic_free(omni_calibration*);

#endif /* OMNIC_CALIBRATION_H_ */

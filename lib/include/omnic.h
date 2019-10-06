/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef OMNIC_H_
#define OMNIC_H_

/// Error codes
enum omnic_error_code {
    OMNIC_OK = 0,
    OMNIC_ERROR_READ = 1,
    OMNIC_ERROR_WRITE = 2
};

/// Calibrated pixel as UVDB texture coordinates
typedef struct omnic_uvdb_pixel {
    uint16_t u,v; // Texture coordinates
    uint16_t d;
    uint16_t b; // Blend mask value
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

typedef struct omnic_colorcorrection_lot {
    // Look up table size (1024 by default)
    uint32_t size; 
  
    /// Color correction look up table for red channel
    float* red_data;
    
    /// Color correction look up table for green channel
    float* green_data;
    
    /// Color correction look up table for blue channel
    float* blue_data;
    
    /// Overall color correction for all three channels
    float* all_data;
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
    omnic_colorcorrection_lot colorcorrection;

    /// Buffer width
    uint32_t pixeldata_width;

    /// Buffer Height
    uint32_t pixeldata_height;

    /// Buffer with calibrated pixels
    omni_uvdb_pixel* pixeldata;
};

/// Calibration for several projectors
typedef struct omnic_calibration {
    /// Header string
    char header[80];

    uint32_t major_version;
    uint32_t minor_version;
    uint32_t patch_version;

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

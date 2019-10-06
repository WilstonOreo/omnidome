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
#ifndef OMNI_PROJ_CALIBRATIONMODE_H_
#define OMNI_PROJ_CALIBRATIONMODE_H_

#include <omni/serialization/Interface.h>
#include <omni/mapping/Interface.h>

namespace omni {
  namespace proj {
    /// Defines which type of data is in a calibration
    enum class CalibrationMode {
      TEXCOORDS, UVW, MAPPED_INPUT
    };

    inline static mapping::OutputMode calibrationModeToMappingOutputMode(
        CalibrationMode _calibMode) {
      switch (_calibMode) {
        default:
        case CalibrationMode::TEXCOORDS:
          return mapping::OutputMode::TEXCOORDS;
        case CalibrationMode::UVW:
          return mapping::OutputMode::UVW;
        case CalibrationMode::MAPPED_INPUT:
          return mapping::OutputMode::MAPPED_INPUT;
      }
      // Return texcoords by default
      return mapping::OutputMode::TEXCOORDS;
    }

    inline static int imageHeightMultiplier(CalibrationMode _calibMode) {
      switch (_calibMode) {
        default:
        case CalibrationMode::TEXCOORDS:
          return 2;
        case CalibrationMode::UVW:
          return 3;
        case CalibrationMode::MAPPED_INPUT:
          return 1;
      }
      return 2;
    }
  }

  using proj::CalibrationMode;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::proj::CalibrationMode)

#endif /* OMNI_PROJ_CALIBRATIONMODE_H_ */

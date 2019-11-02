/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_RENDER_EXPORTSETTINGS_H_
#define OMNI_RENDER_EXPORTSETTINGS_H_

#include <omni/mapping/Interface.h>
#include <omni/serialization/Interface.h>
#include <omni/proj/CalibrationMode.h>

namespace omni {
  namespace render {
    /// Mode how calibrations are stitched together
    enum class SeparationMode
    {
      NONE,
      SCREENS,
      PROJECTORS
    };

    enum class OutputType {
      OMNI_CALIBRATION,
      PLAIN_IMAGE
    };

    /// Class for defining render options for calibration data
    class OMNI_EXPORT ExportSettings {
      public:
        /**@brief Mapping output mode
           @detail Determines if uvw map, texture coordinates or the actual
                   mapped image shall be output.
         **/
        CalibrationMode outputMode() const;

        /// Converts calibration mode into mapping output mode
        mapping::OutputMode mappingOutputMode() const;

        /**@brief Set Mapping output mode
           @detail Determines if uvw map, texture coordinates or the actual
                   mapped image shall be output.
         **/
        void setOutputMode(CalibrationMode);

        /**@brief Determines how output images are separated
           @detail Images can be stitched together or separated by screen or
              projector.
                   Ignored when OMNI_CALIBRATION is output type.
         **/
        SeparationMode separationMode() const;

        /**@brief Set separation mode which defines how output images are
           separated
           @detail Images can be stitched together or separated by screen or
              projector.
                   Ignored when OMNI_CALIBRATION is output type.
         **/
        void setSeparationMode(SeparationMode);

        /// Projectors that are not assigned to a screen are excluded
        bool excludeUnassignedProjectors() const;

        /// Set flag for to exclude projectors that are not assigned to a screen
        void setExcludeUnassignedProjectors(bool);

        /// Output type can be a plain image (png) or omnidome calibration
        // format
        OutputType outputType() const;

        /// Output type can be a plain image (png) or omnidome calibration
        // format
        void setOutputType(OutputType);

        /// Deserialize from stream
        void        fromStream(QDataStream&);

        /// Serialize to stream
        void        toStream(QDataStream&) const;

        /// Test for equality. ScreenSetup is ignored
        friend bool operator==(ExportSettings const&,
                               ExportSettings const&);

      private:
        OutputType outputType_            = OutputType::OMNI_CALIBRATION;
        CalibrationMode outputMode_       = CalibrationMode::TEXCOORDS;
        SeparationMode  separationMode_   = SeparationMode::SCREENS;
        bool excludeUnassignedProjectors_ = false;
    };
  }

  using render::ExportSettings;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::render::OutputType)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::render::SeparationMode)

OMNI_DECL_STREAM_OPERATORS(omni::render::ExportSettings)


#endif /* OMNI_EXPORTSETTINGS_H_ */

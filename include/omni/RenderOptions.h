/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#ifndef OMNI_RENDEROPTIONS_H_
#define OMNI_RENDEROPTIONS_H_

#include <omni/mapping/Interface.h>

namespace omni
{
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

  enum class OutputMode {
    MAPPED_OUTPUT,
    UVW_MAP,
    TEXCOORDS,
    BLENDMASK
  };

  /// Class for defining render options for calibration data
  class RenderOptions
  {
  public:
    /**@brief Mapping output mode
       @detail Determines if uvw map, texture coordinates or the actual
               mapped image shall be output.
     **/
    mapping::OutputMode mappingOutputMode() const;

    /**@brief Set Mapping output mode
       @detail Determines if uvw map, texture coordinates or the actual
               mapped image shall be output.
     **/
    void setMappingOutputMode(mapping::OutputMode);

    /**@brief Determines how output images are separated
       @detail Images can be stitched together or separated by screen or projector.
               Ignored when OMNI_CALIBRATION is output type.
     **/
    SeparationMode separationMode() const;

    /**@brief Set separation mode which defines how output images are separated
       @detail Images can be stitched together or separated by screen or projector.
               Ignored when OMNI_CALIBRATION is output type.
     **/
    void setSeparationMode(SeparationMode);

    /// Projectors that are not assigned to a screen are excluded
    bool excludeUnassignedProjectors() const;

    /// Set flag for to exclude projectors that are not assigned to a screen
    void setExcludeUnassignedProjectors(bool);

    /// Output type can be a plain image (png) or omnidome calibration format
    OutputType outputType() const;

    /// Output type can be a plain image (png) or omnidome calibration format
    void setOutputType(OutputType);

  private:
    OutputType outputType_ = OutputType::OMNI_CALIBRATION;
    mapping::OutputMode mappingOutputMode_ = mapping::OutputMode::TEXCOORDS;
    SeparationMode separationMode_ = SeparationMode::SCREENS;
    bool excludeUnassignedProjectors_ = false;
  };
}


#endif /* OMNI_RENDEROPTIONS_H_ */

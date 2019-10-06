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

#include <omni/render/ExportSettings.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
  namespace render {
    CalibrationMode ExportSettings::outputMode() const
    {
      return outputMode_;
    }

    mapping::OutputMode ExportSettings::mappingOutputMode() const {
      return proj::calibrationModeToMappingOutputMode(outputMode_);
    }

    void ExportSettings::setOutputMode(CalibrationMode _outputMode)
    {
      outputMode_ = _outputMode;
    }

    SeparationMode ExportSettings::separationMode() const
    {
      return separationMode_;
    }

    void ExportSettings::setSeparationMode(SeparationMode _separationMode)
    {
      separationMode_ = _separationMode;
    }

    bool ExportSettings::excludeUnassignedProjectors() const
    {
      return excludeUnassignedProjectors_;
    }

    void ExportSettings::setExcludeUnassignedProjectors(
      bool _excludeUnassignedProjectors)
    {
      excludeUnassignedProjectors_ = _excludeUnassignedProjectors;
    }

    OutputType ExportSettings::outputType() const {
      return outputType_;
    }

    void ExportSettings::setOutputType(OutputType _outputType) {
      outputType_ = _outputType;
    }

    void ExportSettings::fromStream(QDataStream& _is) {
      PropertyMap _map;

      _is >> _map;
      _map.get("outputType", outputType_);
      _map.get("outputMode", outputMode_);
      _map.get("separationMode", separationMode_);
      _map.get("excludeUnassignedProjectors", excludeUnassignedProjectors_);
    }

    void ExportSettings::toStream(QDataStream& _os) const {
      PropertyMap _map;

      _map("outputType", outputType_);
      _map("outputMode", outputMode_);
      _map("separationMode", separationMode_);
      _map("excludeUnassignedProjectors", excludeUnassignedProjectors_);
      _os << _map;
    }

    bool operator==(
      ExportSettings const& _lhs,
      ExportSettings const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(outputType_) &&
        OMNI_TEST_MEMBER_EQUAL(outputMode_) &&
        OMNI_TEST_MEMBER_EQUAL(separationMode_) &&
        OMNI_TEST_MEMBER_EQUAL(excludeUnassignedProjectors_);
    }
  }
}

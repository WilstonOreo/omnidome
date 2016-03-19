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

#include <omni/ui/MappingParameters.h>

namespace omni
{
  namespace ui
  {
    MappingParameters::MappingParameters(QWidget* _parent) :
      ParameterWidget(_parent)
    {
    }

    MappingParameters::MappingParameters(
        mapping::Interface* _mapping,
        QWidget* _parent) :
      ParameterWidget(_parent)
    {
        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        setLayout(_layout);
        setMapping(_mapping);
    }

    MappingParameters::~MappingParameters()
    {
    }

    mapping::Interface* MappingParameters::mapping()
    {
      return mapping_;
    }

    mapping::Interface const* MappingParameters::mapping() const
    {
      return mapping_;
    }

    void MappingParameters::setMapping(mapping::Interface* _mapping)
    {
      mapping_ = _mapping;
    }

    void MappingParameters::addDefaultParameters() {
        if (!mapping()) return;
        addCheckBox("Flip horizontal",mapping()->flipHorizontal());
        addCheckBox("Flip vertical",mapping()->flipVertical());

        if (mapping()->isUVW()) {
            addCheckBox("Bound to canvas",mapping()->isBoundToCanvas());
        }
    }

    void MappingParameters::updateParameters()
    {
      if (!mapping_ || isLocked()) return;

      updateMappingParameters();
      emit parametersUpdated();
    }
  }
}

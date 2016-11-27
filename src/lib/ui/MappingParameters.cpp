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

#include <omni/ui/MappingParameters.h>

namespace omni
{
  namespace ui
  {
    MappingParameters::MappingParameters(QWidget* _parent) :
      ParameterWidget(_parent)
    {
        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSizeConstraint(QLayout::SetMaximumSize);
        setLayout(_layout);
        connect(this,&ParameterWidget::parametersUpdated,this,&MappingParameters::updateDataModel);
    }

    MappingParameters::~MappingParameters()
    {
    }

    void MappingParameters::dataToFrontend() {
        addCheckBox("Flip horizontal",dataModel()->flipHorizontal());
        addCheckBox("Flip vertical",dataModel()->flipVertical());

        if (dataModel()->isUVW()) {
            addCheckBox("Bound to canvas",dataModel()->isBoundToCanvas());
            transform_ = addAffineTransformWidget("Transform", &dataModel()->transform());
        //    transform_->setTranslationVisible(false);
        }
    }

    bool MappingParameters::frontendToData()
    {
        dataModel()->setFlipHorizontal(getParamAsBool("Flip horizontal"));
        dataModel()->setFlipVertical(getParamAsBool("Flip vertical"));

        if (dataModel()->isUVW()) {
            dataModel()->setBoundToCanvas(getParamAsBool("Bound to canvas"));

            if (transform_)
                transform_->updateDataModel();
        }
        return true;
    }
  }
}

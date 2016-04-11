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

#include <omni/ui/CanvasParameters.h>

namespace omni
{
  namespace ui
  {
    CanvasParameters::CanvasParameters(QWidget* _parent) :
      ParameterWidget(_parent)
    {
        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSizeConstraint(QLayout::SetMaximumSize);
        setLayout(_layout);
        connect(this,&ParameterWidget::parametersUpdated,this,&CanvasParameters::updateDataModel);
    }

    CanvasParameters::~CanvasParameters()
    {
    }

    bool CanvasParameters::frontendToData() {
        transform_->updateDataModel();
        return true;
    }

    void CanvasParameters::dataToFrontend() {
        transform_ = addAffineTransformWidget("Transform", &dataModel()->transform());
    }


    void CanvasParameters::setRescaleValues(bool _rescale) {
      ParameterWidget::setRescaleValues(_rescale);
      if (transform_) {
        transform_->setRescaleOffsetValues(_rescale);
      }
      emit dataModelChanged();
    }

    /// Set slider ranges
    void CanvasParameters::setScale(float _scale) {
      ParameterWidget::setScale(_scale);
      if (transform_) {
        transform_->setOffsetRangeScale(_scale);
      }
      emit dataModelChanged();
    }

    /// Set slider ranges
    void CanvasParameters::setUnit(QString const& _unit) {
      ParameterWidget::setUnit(_unit);
      if (transform_) {
        transform_->setOffsetUnit(_unit);
      }
      emit dataModelChanged();
    }
  }
}

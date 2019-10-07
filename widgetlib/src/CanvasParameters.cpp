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

#include <omni/ui/CanvasParameters.h>

#include <omni/visual/Scene.h>
#include <QSignalBlocker>

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
        if (!dataModel()) {
          return false;
        }
        transform_->updateDataModel();
        canvas()->setViewMode(util::intToEnum<Canvas::ViewMode>(boxViewMode_->currentIndex()));
        return true;
    }

    void CanvasParameters::dataToFrontend() {
        if (!transform_) {
          transform_.reset(addAffineTransformWidget("Transform", &canvas()->transform()));
        }

        if (!boxViewMode_) {
            boxViewMode_.reset(new QComboBox());
            boxViewMode_->addItem("Inside");
            boxViewMode_->addItem("Outside");
            boxViewMode_->addItem("Both");
            layout()->addWidget(boxViewMode_.get());
            connect(boxViewMode_.get(),
                static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &CanvasParameters::updateDataModel);
        }
        boxViewMode_->setCurrentIndex(util::enumToInt(canvas()->
                                                               viewMode()));
    }

    void CanvasParameters::setDataModel(Session* s) {
      {
        QSignalBlocker blocker(this);
        mixin_data_model_type::setDataModel(s);
        setScale(scene()->size());
        setUnit(scene()->unit().abbreviation());
        updateFrontend();
      }
      emit dataModelChanged();
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
    }

    visual::Scene* CanvasParameters::scene() const {
        return &dataModel()->scene();

    }

    canvas::Interface* CanvasParameters::canvas() const {
        return dataModel()->canvas();
    }
  }
}

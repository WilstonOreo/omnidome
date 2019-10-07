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

#include "Blend.h"
#include "ui_omni_ui_Blend.h"

#include <QDebug>
#include <QVBoxLayout>

#include <omni/Session.h>
#include <omni/BlendMask.h>
#include <omni/ui/RangedFloat.h>

namespace omni {
  namespace ui {
    Blend::Blend(QWidget *_parent) :
      DockWidget(_parent)
    {
      this->setup(ui_);

      auto setupSlider = [&](RangedFloat *_slider, QString _label)
                         {
                           _slider->setLabel(_label);
                           _slider->setRange(0.0, 0.5);
                           _slider->setSingleStep(0.01);
                           _slider->setPageStep(0.05);
                           _slider->setGripSize(0);
                           connect(_slider,
                                   &RangedFloat::valueChanged,
                                   this,
                                   &Blend::updateDataModel);
                         };

      /// Setup top, left, right and bottom sliders
      {
        setupSlider(ui_->sliderTop,          "Top");
        setupSlider(ui_->sliderLeft,         "Left");
        setupSlider(ui_->sliderRight,        "Right");
        setupSlider(ui_->sliderBottom,       "Bottom");
        setupSlider(ui_->sliderInputOpacity, "Input Opacity");
        ui_->sliderInputOpacity->setRange(0.0, 1.0);
      }

      /// Setup gamma
      {
        setupSlider(ui_->sliderGamma, "Gamma");
        ui_->sliderGamma->setRange(0.0, 3.0);
        ui_->sliderGamma->setSingleStep(0.05);
        ui_->sliderGamma->setPageStep(0.3);
      }

      /// Setup brush widgets
      {
        setupSlider(ui_->sliderSize, "Brush size");
        ui_->sliderSize->setRange(5.0, 500.0);
        ui_->sliderSize->setSingleStep(5.0);
        ui_->sliderSize->setPageStep(50.0);

        setupSlider(ui_->sliderFeather, "Feather");
        ui_->sliderFeather->setRange(0.1, 4.0);
        ui_->sliderFeather->setSingleStep(0.1);
        ui_->sliderFeather->setPageStep(0.5);

        setupSlider(ui_->sliderOpacity, "Opacity");
        ui_->sliderOpacity->setRange(0.0, 1.0);
        ui_->sliderOpacity->setSingleStep(0.01);
        ui_->sliderOpacity->setPageStep(0.1);

        connect(ui_->chkInvert, &QCheckBox::clicked, this,
                &Blend::updateDataModel);
      }

      /// Blend mode combobox
      {
        connect(ui_->boxMaskColor,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::
                                                      currentIndexChanged), this,
                &Blend::updateDataModel);
      }
    }

    Blend::~Blend()
    {}

    void Blend::dataToFrontend()
    {
      if (!blendMask()) return;

      ui_->sliderLeft->setValue(blendMask()->leftWidth());
      ui_->sliderRight->setValue(blendMask()->rightWidth());
      ui_->sliderTop->setValue(blendMask()->topWidth());
      ui_->sliderBottom->setValue(blendMask()->bottomWidth());
      ui_->sliderGamma->setValue(blendMask()->gamma());
      ui_->sliderInputOpacity->setValue(
        dataModel()->blendSettings().inputOpacity());

      auto& _brush = blendMask()->brush();
      ui_->sliderSize->setValue(blendMask()->brushSize());
      ui_->sliderFeather->setValue(_brush.feather());
      ui_->sliderOpacity->setValue(_brush.opacity());
      ui_->chkInvert->setChecked(_brush.invert());
      ui_->brushPreview->update(_brush.feather(),
                                _brush.opacity(), _brush.invert());
    }

    bool Blend::frontendToData()
    {
      if (!blendMask()) return false;

      float _left   = ui_->sliderLeft->value();
      float _right  = ui_->sliderRight->value();
      float _top    = ui_->sliderTop->value();
      float _bottom = ui_->sliderBottom->value();

      blendMask()->setRect(
        QRectF(_left, _top, 1.0 - _right - _left, 1.0 - _top - _bottom));

      blendMask()->setGamma(ui_->sliderGamma->value());

      float _feather = ui_->sliderFeather->value();
      float _opacity = ui_->sliderOpacity->value();
      bool  _invert  = ui_->chkInvert->isChecked();

      blendMask()->setBrush(
        ui_->sliderSize->value(), // Size
        _feather, _opacity, _invert);
      ui_->brushPreview->update(_feather, _opacity, _invert);

      int   _blendModeIndex = ui_->boxMaskColor->currentIndex();
      auto& _blendSettings  = dataModel()->blendSettings();

      if (_blendModeIndex == 0) _blendSettings.setColorMode(
          BlendSettings::ColorMode::COLORED);

      if (_blendModeIndex == 1) _blendSettings.setColorMode(
          BlendSettings::ColorMode::WHITE);

      _blendSettings.setInputOpacity(ui_->sliderInputOpacity->value());

      return true;
    }

    omni::BlendMask* Blend::blendMask() const
    {
      if (!dataModel()) return nullptr;

      return dataModel()->tunings().current() ? &dataModel()->tunings().
             current()->blendMask() : nullptr;
    }
  }
}

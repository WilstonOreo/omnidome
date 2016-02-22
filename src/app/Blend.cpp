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

#include "Blend.h"
#include "ui_omni_ui_Blend.h"

#include <omni/Session.h>
#include <omni/BlendMask.h>

namespace omni
{
  namespace ui
  {
    Blend::Blend(QWidget* _parent) :
      DockWidget(_parent)
    {
      this->setup(ui_);

      /// Setup top, left, right and bottom sliders
      {
        auto setupSlider = [&](RangedFloat* _slider)
        {
          _slider->setRange(0.0,0.5);
          _slider->setSingleStep(0.01);
          _slider->setPageStep(0.05);
          connect(_slider,SIGNAL(valueChanged()),this,SLOT(changeBlendMask()));
        };

        setupSlider(ui_->sliderTop);
        ui_->sliderTop->setLabel("Top");
        setupSlider(ui_->sliderLeft);
        ui_->sliderLeft->setLabel("Left");
        setupSlider(ui_->sliderRight);
        ui_->sliderRight->setLabel("Right");
        setupSlider(ui_->sliderBottom);
        ui_->sliderBottom->setLabel("Bottom");
      }

      /// Setup gamma
      {
        ui_->sliderGamma->setLabel("Gamma");
        ui_->sliderGamma->setRange(0.0,3.0);
        ui_->sliderGamma->setSingleStep(0.05);
        ui_->sliderGamma->setPageStep(0.3);
        connect(ui_->sliderGamma,SIGNAL(valueChanged()),this,SLOT(changeBlendMask()));
      }

      /// Setup brush widgets
      {
        ui_->sliderSize->setRange(5.0,500.0);
        ui_->sliderSize->setSingleStep(5.0);
        ui_->sliderSize->setPageStep(50.0);
        connect(ui_->sliderSize,SIGNAL(valueChanged()),this,SLOT(changeBlendMask()));
        ui_->sliderSize->setLabel("Brush size");

        ui_->sliderFeather->setRange(0.1,4.0);
        ui_->sliderFeather->setSingleStep(0.1);
        ui_->sliderFeather->setPageStep(0.5);
        connect(ui_->sliderFeather,SIGNAL(valueChanged()),this,SLOT(changeBlendMask()));
        ui_->sliderFeather->setLabel("Feather");
        connect(ui_->chkInvert,SIGNAL(clicked(bool)),this,SLOT(changeBlendMask()));
      }

      /// Blend mode combobox
      {
        connect(ui_->boxMaskColor,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBlendMask()));
      }
    }

    Blend::~Blend()
    {
    }

    Session const* Blend::session() const
    {
      return session_;
    }

    void Blend::setSession(Session* _session)
    {
      session_=_session;
      updateBlendMask();
    }

    void Blend::updateBlendMask()
    {
      if (blendMask())
      {
        locked_ = true;
        ui_->sliderLeft->setValue(blendMask()->leftWidth());
        ui_->sliderRight->setValue(blendMask()->rightWidth());
        ui_->sliderTop->setValue(blendMask()->topWidth());
        ui_->sliderBottom->setValue(blendMask()->bottomWidth());
        ui_->sliderGamma->setValue(blendMask()->gamma());

        auto& _brush = blendMask()->brush();
        ui_->sliderSize->setValue(_brush.size());
        ui_->sliderFeather->setValue(_brush.feather());
        ui_->chkInvert->setChecked(_brush.invert());
        ui_->brushPreview->update(_brush.feather(),_brush.invert());
        locked_ = false;
      }
    }

    void Blend::changeBlendMask()
    {
      if (!blendMask() || locked_) return;

      float _left = ui_->sliderLeft->value();
      float _right = ui_->sliderRight->value();
      float _top = ui_->sliderTop->value();
      float _bottom = ui_->sliderBottom->value();

      blendMask()->setRect(
        QRectF(_left,_top,1.0 - _right - _left,1.0 - _top - _bottom));

      blendMask()->setGamma(ui_->sliderGamma->value());

      float _feather = ui_->sliderFeather->value();
      bool _invert = ui_->chkInvert->isChecked();

      blendMask()->brush().setBrush(
        ui_->sliderSize->value(), // Size
        _feather,_invert);

      ui_->brushPreview->update(_feather,_invert);

      int _blendModeIndex = ui_->boxMaskColor->currentIndex();
      if (_blendModeIndex == 0)
        session_->setBlendMode(Session::BlendMode::COLOR);
      if (_blendModeIndex == 1)
        session_->setBlendMode(Session::BlendMode::WHITE);
      if (_blendModeIndex == 2)
        session_->setBlendMode(Session::BlendMode::INPUT);

      emit blendMaskChanged();
    }

    omni::BlendMask const* Blend::blendMask() const
    {
      if (!session_) return nullptr;

      return session_->tunings().current() ? &session_->tunings().current()->blendMask() : nullptr;
    }

    omni::BlendMask* Blend::blendMask()
    {
      if (!session_) return nullptr;

      return session_->tunings().current() ? &session_->tunings().current()->blendMask() : nullptr;
    }
  }
}

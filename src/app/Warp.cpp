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

#include "Warp.h"

#include "ui_omni_ui_Warp.h"
#include <QMessageBox>

#include <omni/Session.h>
#include <omni/WarpGrid.h>
#include <omni/util.h>

namespace omni
{
  namespace ui
  {
    Warp::Warp(QWidget* _parent) :
      DockWidget(_parent)
    {
        this->setup(ui_);

      connect(ui_->btnResize,SIGNAL(clicked(bool)),this,SLOT(resizeWarpGrid(bool)));
      connect(ui_->btnReset,SIGNAL(clicked()),this,
              SLOT(resetWarpGrid()));

      connect(ui_->boxInterpolation,SIGNAL(currentIndexChanged(int)),this,
              SLOT(changeInterpolation(int)));
      connect(ui_->chkShowBlendMask,SIGNAL(clicked(bool)),this,SLOT(setShowBlendMask(bool)));

      auto setupSlider = [&](RangedInt* _slider)
      {
        _slider->hide();
        _slider->setRange(2,12);
        _slider->setSingleStep(1);
        _slider->setPageStep(12);
        _slider->setDefaultValue(6);
        _slider->setUseDefaultValue(true);
        connect(_slider,SIGNAL(valueChanged()),this,SLOT(resizeWarpGrid()));
      };

      setupSlider(ui_->sliderHorz);
      ui_->sliderHorz->setLabel("Horizontal");

      setupSlider(ui_->sliderVert);
      ui_->sliderVert->setLabel("Vertical");

    }

    Warp::~Warp()
    {
    }

    void Warp::sessionParameters() {

        ui_->chkShowBlendMask->setChecked(session()->blendSettings().showInWarpMode());
      updateWarpGrid();
    }

    void Warp::updateWarpGrid()
    {
      if (!warpGrid()) return;

      this->locked([&]() {
            ui_->sliderHorz->setValue(warpGrid()->horizontal());
            ui_->sliderVert->setValue(warpGrid()->vertical());
        });
        emit warpGridChanged();
    }
    void Warp::setShowBlendMask(bool _show) {
        if (!session() || this->isLocked()) return;

        session()->blendSettings().setShowInWarpMode(_show);
        emit warpGridChanged();
    }

    void Warp::changeInterpolation(int _index) {
        if (!warpGrid()) return;

        auto _interp = util::intToEnum<WarpGrid::Interpolation>(_index);
        warpGrid()->setInterpolation(_interp);
        emit warpGridChanged();
    }

    void Warp::resetWarpGrid()
    {
      if (!warpGrid() || this->isLocked()) return;

      warpGrid()->reset();

      emit warpGridChanged();
    }

    void Warp::resizeWarpGrid(bool _enabled)
    {
      if (!warpGrid() || this->isLocked()) return;

      if (_enabled && !warpGrid()->isReset())
      {
        QMessageBox::StandardButton _reply =
          QMessageBox::question(this,"Reset warp grid",
                                "Changing the warp grid resolution resets all warp grid points. Do you want to continue?",
                                QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if (_reply == QMessageBox::No)
        {
          ui_->btnResize->setChecked(false);
          return;
        }
      }

      ui_->sliderVert->setVisible(_enabled);
      ui_->sliderHorz->setVisible(_enabled);
    }

    void Warp::resizeWarpGrid()
    {
      if (!warpGrid() || this->isLocked()) return;

      warpGrid()->resize(ui_->sliderHorz->value(),ui_->sliderVert->value());
      emit warpGridChanged();
    }

    omni::WarpGrid const* Warp::warpGrid() const
    {
      if (!session()) return nullptr;

      return session()->tunings().current() ? &session()->tunings().current()->warpGrid() : nullptr;
    }

    omni::WarpGrid* Warp::warpGrid()
    {
      if (!session()) return nullptr;

      return session()->tunings().current() ? &session()->tunings().current()->warpGrid() : nullptr;
    }
  }
}

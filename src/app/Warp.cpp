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

#include "Warp.h"

#include "ui_omni_ui_Warp.h"
#include <QMessageBox>

#include <omni/Session.h>
#include <omni/WarpGrid.h>
#include <omni/util.h>
#include <omni/visual/Tuning.h>

namespace omni {
  namespace ui {
    Warp::Warp(QWidget *_parent) :
      DockWidget(_parent)
    {
      this->setup(ui_);

      auto setupSlider = [&](RangedInt *_slider)
                         {
                           _slider->hide();
                           _slider->setRange(2, 12);
                           _slider->setSingleStep(1);
                           _slider->setPageStep(1);
                           _slider->setDefaultValue(4);
                           _slider->setUseDefaultValue(true);
                           connect(_slider, &RangedInt::valueChanged,
                                   this, &Warp::updateDataModel);
                         };

      setupSlider(ui_->sliderHorz);
      ui_->sliderHorz->setLabel("Horizontal");

      setupSlider(ui_->sliderVert);
      ui_->sliderVert->setLabel("Vertical");

      connect(this, &Warp::dataModelChanged, this, &Warp::updateFrontend);

      connect(ui_->btnResize, SIGNAL(clicked(bool)), this,
              SLOT(resizeWarpGrid(bool)));
      connect(ui_->btnReset, SIGNAL(clicked()), this,
              SLOT(resetWarpGrid()));

      connect(ui_->boxInterpolation, SIGNAL(currentIndexChanged(int)), this,
              SLOT(changeInterpolation(int)));
      connect(ui_->chkShowBlendMask, &QCheckBox::clicked,
              this, &Warp::updateDataModel);
    }

    Warp::~Warp()
    {}

    void Warp::dataToFrontend()
    {
      if (!warpGrid()) return;

      ui_->sliderHorz->setValue(warpGrid()->horizontal());
      ui_->sliderVert->setValue(warpGrid()->vertical());
      ui_->chkShowBlendMask->setChecked(
        dataModel()->blendSettings().showInWarpMode());
    }

    bool Warp::frontendToData() {
      if (!warpGrid()) return false;

      dataModel()->blendSettings().setShowInWarpMode(
        ui_->chkShowBlendMask->isChecked());
      warpGrid()->resize(ui_->sliderHorz->value(), ui_->sliderVert->value());

      return true;
    }

    void Warp::changeInterpolation(int _index) {
      if (!warpGrid()) return;

      auto _interp = util::intToEnum<WarpGrid::Interpolation>(_index);
      warpGrid()->setInterpolation(_interp);
      dataModel()->tunings().current()->visualizer()->updateWarpGrid();
      updateFrontend();
      emit dataModelChanged();
    }

    void Warp::resetWarpGrid()
    {
      if (!warpGrid() || signalsBlocked()) return;

      warpGrid()->reset();
      dataModel()->tunings().current()->visualizer()->updateWarpGrid();
      emit dataModelChanged();
    }

    void Warp::resizeWarpGrid(bool _enabled)
    {
      if (!warpGrid()) return;

      if (_enabled && !warpGrid()->isReset())
      {
        QMessageBox::StandardButton _reply =
          QMessageBox::question(this,
                                "Reset warp grid",
                                "Changing the warp grid resolution resets all warp grid points. Do you want to continue?",
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);

        if (_reply == QMessageBox::No)
        {
          ui_->sliderVert->hide();
          ui_->sliderHorz->hide();
          ui_->btnResize->setChecked(false);
          return;
        }
      }

      ui_->sliderVert->show();
      ui_->sliderHorz->show();
    }

    omni::WarpGrid const * Warp::warpGrid() const
    {
      if (!dataModel()) return nullptr;

      return dataModel()->tunings().current() ? &dataModel()->tunings().current()
             ->warpGrid() : nullptr;
    }

    omni::WarpGrid * Warp::warpGrid()
    {
      if (!dataModel()) return nullptr;

      return dataModel()->tunings().current() ? &dataModel()->tunings().current()
             ->warpGrid() : nullptr;
    }
  }
}

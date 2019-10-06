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

#include "Scene.h"

#include "ui_omni_ui_Scene.h"

#include <omni/util.h>
#include <QSignalBlocker>

namespace omni {
  namespace ui {
    Scene::Scene(QWidget *_parent) :
      DockWidget(_parent),
      ui_(new Ui::Scene)
    {
      this->setup(ui_);

      QSignalBlocker blocker(this);
      {
        connect(ui_->chkInput,
                &QCheckBox::clicked,
                this,
                &Scene::updateDataModel);
        connect(ui_->chkGrid, &QCheckBox::clicked, this, &Scene::updateDataModel);
        connect(ui_->boxProjectors,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &Scene::updateDataModel);
        connect(ui_->boxProjectedAreas,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &Scene::updateDataModel);

        connect(ui_->boxProjectedAreas,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &Scene::updateDataModel);

        ui_->sliderInsideOutside->setRange(0.0,1.0);
        ui_->sliderInsideOutside->setGripSize(0.0);
        ui_->sliderInsideOutside->setPageStep(0.1);
        ui_->sliderInsideOutside->setSingleStep(0.01);

        connect(ui_->sliderInsideOutside, &RangedFloat::valueChanged,
                this, &Scene::updateDataModel);

        ui_->sliderFOV->setRange(10.0,100.0);
        ui_->sliderFOV->setLabel("FOV");
        ui_->sliderFOV->setGripSize(0.0);
        ui_->sliderFOV->setPageStep(1.0);
        ui_->sliderFOV->setSingleStep(0.1);

        connect(ui_->sliderFOV, &RangedFloat::valueChanged,
                this, &Scene::updateDataModel);

        ui_->sliderWireframe->setRange(0.0,1.0);
        ui_->sliderWireframe->setGripSize(0.0);
        ui_->sliderInsideOutside->setPageStep(0.1);
        ui_->sliderInsideOutside->setSingleStep(0.01);

        connect(ui_->sliderWireframe, &RangedFloat::valueChanged,
                this, &Scene::updateDataModel);

        connect(ui_->boxSize, SIGNAL(currentIndexChanged(int)), this,
                SLOT(setSceneScale()));
        connect(ui_->boxUnit, SIGNAL(currentIndexChanged(int)), this,
                SLOT(setUnit()));
        connect(ui_->btnFitToCanvas, SIGNAL(clicked()), this,
                SLOT(fitSceneSizeToCanvas()));

        sceneSizeWidgets_ = {
          ui_->boxSize,
          ui_->boxUnit,
          ui_->btnFitToCanvas,
          ui_->lbSceneSize,
          ui_->lbUnit,
          ui_->chkRescaleValues
        };
      }
    }

    Scene::~Scene() {
    }

    void Scene::showEvent(QShowEvent* _event) {
      if (!dataModel()) return;

      switch(dataModel()->mode()) {
        case Session::Mode::ARRANGE:
        showArrangeMode();
        break;
        default:
        case Session::Mode::LIVE:
        showLiveMode();
        break;
      }
    }

    void Scene::showArrangeMode() {
      for (auto _widget :sceneSizeWidgets_) {
        _widget->show();
      }
      show();
    }

    void Scene::showLiveMode() {
      /// Display only the read only settings in live mode
      for (auto _widget :sceneSizeWidgets_) {
        _widget->hide();
      }
      show();
    }

    void Scene::fitSceneSizeToCanvas() {
      if (!dataModel() || signalsBlocked()) return;

      if (!dataModel()->canvas()) return;

      float _canvasSize = dataModel()->canvas()->radius() * 2.0;

      int _index = ui_->boxSize->count() - 1;

      for (int i = 0; i < ui_->boxSize->count(); ++i) {
        float _value = ui_->boxSize->itemText(i).toFloat();
        if (_value > _canvasSize) {
          _index = i;
          break;
        }
      }

      dataModel()->scene().setSize(ui_->boxSize->itemText(_index).toFloat());
      {
        QSignalBlocker blocker(this);
        ui_->boxSize->setCurrentIndex(_index);
      }

      emit sceneScaleChanged(false);
    }

    void Scene::setSceneScale() {
      if (!dataModel() || signalsBlocked()) return;

      auto& _scene = dataModel()->scene();
      _scene.setSize(ui_->boxSize->currentText().toFloat());

      emit sceneScaleChanged(ui_->chkRescaleValues->isChecked());
    }

    void Scene::setUnit() {
      if (!dataModel() || signalsBlocked()) return;

      auto& _scene = dataModel()->scene();
      auto && _stringList = ui_->boxUnit->currentText().split(" ");

      if (_stringList.isEmpty()) return;

      _scene.setUnit(_stringList[0]);

      emit unitChanged();
    }

    void Scene::dataToFrontend() {
      auto& _scene = dataModel()->scene();

      ui_->sliderInsideOutside->setValue(_scene.insideOutside());
      ui_->sliderFOV->setValue(_scene.camera()->fov());
      ui_->sliderWireframe->setValue(_scene.wireframe());

      ui_->chkInput->setChecked(_scene.displayInput());
      ui_->chkGrid->setChecked(_scene.displayGrid());
      ui_->boxProjectors->setCurrentIndex(util::enumToInt(_scene.displayProjectors()));
      ui_->boxProjectedAreas->setCurrentIndex(util::enumToInt(_scene.displayProjectedAreas()));

      // Set current scene size to combobox
      {
        int   _index   = -1;
        float _minDist = std::numeric_limits<float>::max();

        for (int i = 0; i < ui_->boxSize->count(); ++i) {
          float _value = ui_->boxSize->itemText(i).toFloat();

          if (std::abs(_value - _scene.size()) < _minDist) {
            _minDist = std::abs(_value - _scene.size());
            _index   = i;
          }
        }
        dataModel()->scene().setSize(ui_->boxSize->itemText(_index).toFloat());
        ui_->boxSize->setCurrentIndex(_index);
      }

      // Set current scene unit to combobox
      {
        int _index = 0;
        QString _abbr("m");

        for (int i = 0; i < ui_->boxUnit->count(); ++i) {
          QString _itemText = ui_->boxUnit->itemText(i);
          _abbr = _itemText.split(' ')[0];

          if (_abbr == _scene.unit().abbreviation()) {
            _index = i;
            break;
          }
        }
        dataModel()->scene().setUnit(_abbr);
        ui_->boxUnit->setCurrentIndex(_index);
        emit sceneScaleChanged(false);
        emit unitChanged();
      }
    }

    bool Scene::frontendToData() {
      auto& _scene = dataModel()->scene();

      _scene.camera()->setFov(ui_->sliderFOV->value());
      _scene.setInsideOutside(ui_->sliderInsideOutside->value());
      _scene.setDisplayInput(ui_->chkInput->isChecked());
      _scene.setDisplayGrid(ui_->chkGrid->isChecked());
      _scene.setDisplayProjectors(util::intToEnum<visual::ProjectorSelectionMode>(ui_->boxProjectors->currentIndex()));
      _scene.setDisplayProjectedAreas(util::intToEnum<visual::ProjectorSelectionMode>(ui_->boxProjectedAreas->currentIndex()));
      _scene.setWireframe(ui_->sliderWireframe->value());

      return true;
    }
  }
}

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

#include "Scene.h"

#include "ui_omni_ui_Scene.h"


namespace omni {
  namespace ui {
    Scene::Scene(QWidget *_parent) :
      DockWidget(_parent),
      ui_(new Ui::Scene)
    {
      this->setup(ui_);

      this->locked([&]() {
        connect(ui_->chkInput,
                &QCheckBox::clicked,
                this,
                &Scene::updateDataModel);
        connect(ui_->chkGrid, &QCheckBox::clicked, this, &Scene::updateDataModel);
        connect(ui_->chkProjectors, &QCheckBox::clicked, this,
                &Scene::updateDataModel);
        connect(ui_->chkProjectedAreas, &QCheckBox::clicked,
                this, &Scene::updateDataModel);
        connect(ui_->boxFrustumViewMode,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::
                                                      currentIndexChanged),
                this, &Scene::updateDataModel);

        connect(ui_->boxSize, SIGNAL(currentIndexChanged(int)), this,
                SLOT(setSceneScale()));
        connect(ui_->boxUnit, SIGNAL(currentIndexChanged(int)), this,
                SLOT(setUnit()));
        connect(ui_->btnFitToCanvas, SIGNAL(clicked()), this,
                SLOT(fitSceneSizeToCanvas()));
      });
    }

    Scene::~Scene() {}

    void Scene::fitSceneSizeToCanvas() {
      if (!dataModel() || isLocked()) return;

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
      this->locked([&]() {
        ui_->boxSize->setCurrentIndex(_index);
      });
      emit sceneScaleChanged(false);
    }

    void Scene::setSceneScale() {
      if (!dataModel() || isLocked()) return;

      auto& _scene = dataModel()->scene();
      _scene.setSize(ui_->boxSize->currentText().toFloat());

      emit sceneScaleChanged(ui_->chkRescaleValues->isChecked());
    }

    void Scene::setUnit() {
      if (!dataModel() || isLocked()) return;

      auto& _scene = dataModel()->scene();
      auto && _stringList = ui_->boxUnit->currentText().split(" ");

      if (_stringList.isEmpty()) return;

      _scene.setUnit(_stringList[0]);

      emit unitChanged();
    }

    void Scene::dataToFrontend() {
      auto& _scene = dataModel()->scene();

      ui_->chkInput->setChecked(_scene.displayInput());
      ui_->chkGrid->setChecked(_scene.displayGrid());
      ui_->chkProjectors->setChecked(_scene.displayProjectors());
      ui_->chkProjectedAreas->setChecked(_scene.displayProjectedAreas());
      ui_->boxFrustumViewMode->setCurrentIndex(util::enumToInt(_scene.
                                                               projectorViewMode()));

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
      }
    }

    bool Scene::frontendToData() {
      auto& _scene = dataModel()->scene();

      _scene.setDisplayInput(ui_->chkInput->isChecked());
      _scene.setDisplayGrid(ui_->chkGrid->isChecked());
      _scene.setDisplayProjectors(ui_->chkProjectors->isChecked());
      _scene.setDisplayProjectedAreas(ui_->chkProjectedAreas->isChecked());
      _scene.setProjectorViewMode(util::intToEnum<ProjectorViewMode>(ui_->
                                                                     boxFrustumViewMode
                                                                     ->
                                                                     currentIndex()));

      return true;
    }
  }
}

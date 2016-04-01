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
        Scene::Scene(QWidget* _parent) :
            DockWidget(_parent),
            ui_(new Ui::Scene)
        {
            this->setup(ui_);

            connect(ui_->chkInput,&QCheckBox::clicked,this,&Scene::updateDataModel);
            connect(ui_->chkGrid,&QCheckBox::clicked,this,&Scene::updateDataModel);
            connect(ui_->chkProjectors,&QCheckBox::clicked,this,&Scene::updateDataModel);
            connect(ui_->chkProjectedAreas,&QCheckBox::clicked,
                    this,&Scene::updateDataModel);
            connect(ui_->boxFrustumViewMode,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this,&Scene::updateDataModel);

            connect(ui_->boxSize,SIGNAL(currentIndexChanged(int)),this,SLOT(setSceneScale()));
            connect(ui_->boxUnit,SIGNAL(currentIndexChanged(int)),this,SLOT(setUnit()));
        }

        Scene::~Scene() {

        }

        void Scene::setSceneScale() {
            if (!dataModel() || isLocked()) return;

            auto& _scene = dataModel()->scene();
            _scene.setScale(ui_->boxSize->currentText().toFloat());

            emit sceneScaleChanged();
        }

        void Scene::setUnit() {
            if (!dataModel() || isLocked()) return;

            emit unitChanged();
        }

        void Scene::dataToFrontend() {
            auto& _scene = dataModel()->scene();

            ui_->chkInput->setChecked(_scene.displayInput());
            ui_->chkGrid->setChecked(_scene.displayGrid());
            ui_->chkProjectors->setChecked(_scene.displayProjectors());
            ui_->chkProjectedAreas->setChecked(_scene.displayProjectedAreas());
            ui_->boxFrustumViewMode->setCurrentIndex(util::enumToInt(_scene.projectorViewMode()));
        }

        bool Scene::frontendToData() {
            auto& _scene = dataModel()->scene();

            _scene.setDisplayInput(ui_->chkInput->isChecked());
            _scene.setDisplayGrid(ui_->chkGrid->isChecked());
            _scene.setDisplayProjectors(ui_->chkProjectors->isChecked());
            _scene.setDisplayProjectedAreas(ui_->chkProjectedAreas->isChecked());
            _scene.setProjectorViewMode(util::intToEnum<ProjectorViewMode>(ui_->boxFrustumViewMode->currentIndex()));

            return true;
        }
    }
}

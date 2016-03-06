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

#include <omni/ui/GLView3D.h>

#include "ui_omni_ui_Scene.h"


namespace omni {
    namespace ui {
        Scene::Scene(QWidget* _parent) :
            DockWidget(_parent),
            ui_(new Ui::Scene)
        {
            this->setup(ui_);
        }

        Scene::~Scene() {

        }

        void Scene::registerView(GLView3D* _glView) {
            if (views_.count(_glView) != 0) {
                return;
            }

            connect(ui_->chkInput,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayInput(bool)));
            connect(ui_->chkGrid,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayGrid(bool)));
            connect(ui_->chkProjectors,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayProjectors(bool)));
            connect(ui_->chkProjectedAreas,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayProjectedAreas(bool)));
            connect(ui_->boxFrustumViewMode,SIGNAL(currentIndexChanged(int)),_glView,SLOT(setProjectorViewMode(int)));

            views_.insert(_glView);
        }
        void Scene::unregisterView(GLView3D* _glView) {
            if (views_.count(_glView) == 0) {
                return;
            }

            disconnect(ui_->chkInput,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayInput(bool)));
            disconnect(ui_->chkGrid,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayGrid(bool)));
            disconnect(ui_->chkProjectors,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayProjectors(bool)));
            disconnect(ui_->chkProjectedAreas,SIGNAL(clicked(bool)),_glView,SLOT(setDisplayProjectedAreas(bool)));
            disconnect(ui_->boxFrustumViewMode,SIGNAL(currentIndexChanged(int)),_glView,SLOT(setProjectorViewMode(int)));

            views_.erase(_glView);
        }
    }
}

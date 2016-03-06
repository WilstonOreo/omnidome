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

#include "Arrange.h"
#include "ui_omni_ui_Arrange.h"

namespace omni {
    namespace ui {
        Arrange::Arrange(QWidget* _parent) :
            QWidget(_parent),
            ui_(new Ui::Arrange)
        {
            ui_->setupUi(this);
        }

        Arrange::~Arrange() {

        }

        void Arrange::setSession(Session* _session) {
            ui_->view->setSession(_session);
            SessionWidget::setSession(_session);
        }

        GLView3D* Arrange::view() {
            return ui_->view;
        }

        GLView3D const* Arrange::view() const {
            return ui_->view;
        }

        void Arrange::sessionParameters() {

        }

    }
}

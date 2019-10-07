/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#include "MeshWidget.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>


namespace omni {
    namespace ui {
        namespace canvas {

            Mesh::Mesh(QWidget* _parent) :
                ui::CanvasParameters(_parent),
                lastDir_(".")
            {
            }

            Mesh::~Mesh() {

            }

            Mesh::canvas_type* Mesh::mesh() {
                return static_cast<omni::canvas::Mesh*>(dataModel()->canvas());
            }

            Mesh::canvas_type const* Mesh::mesh() const {
                return static_cast<omni::canvas::Mesh const*>(dataModel()->canvas());
            }

            bool Mesh::frontendToData() {
                chkCenter_->setChecked(mesh()->centerToScene());
                chkScale_->setChecked(mesh()->scaleToScene());
                return CanvasParameters::frontendToData();
            }

            void Mesh::centerMesh(bool _center) {
              mesh()->setCenterToScene(_center);
              emit dataModelChanged();
            }

            void Mesh::scaleMesh(bool _scale) {
              mesh()->setScaleToScene(_scale);
              emit dataModelChanged();
            }

            void Mesh::dataToFrontend() {
              if (!btnLoad_) {
                btnLoad_.reset(new QPushButton("Load mesh from file"));
                connect(btnLoad_.get(),SIGNAL(clicked()),this,SLOT(loadMesh()));
                layout()->addWidget(btnLoad_.get());
              }

              if (!chkCenter_) {
                chkCenter_.reset(new QCheckBox("Center mesh to scene"));
                chkCenter_->setChecked(mesh()->centerToScene());
                connect(chkCenter_.get(),SIGNAL(clicked(bool)),this,SLOT(centerMesh(bool)));
                layout()->addWidget(chkCenter_.get());
              }
              if (!chkScale_) {
                chkScale_.reset(new QCheckBox("Scale mesh to scene"));
                chkScale_->setChecked(mesh()->scaleToScene());
                connect(chkScale_.get(),SIGNAL(clicked(bool)),this,SLOT(scaleMesh(bool)));
                layout()->addWidget(chkScale_.get());
              }

              CanvasParameters::dataToFrontend();
            }

            void Mesh::loadMesh() {
                if (!mesh()) return;

                QString _filename = QFileDialog::getOpenFileName(this, "Open mesh file...", lastDir_, "Mesh files (*.obj *.stl *.ply)");

                if (_filename.isEmpty()) return;
                mesh()->load(_filename);

                /// Set last directory
                QFileInfo _info(_filename);
                lastDir_ = _info.absolutePath();
                emit dataModelChanged();
            }
        }
    }
}

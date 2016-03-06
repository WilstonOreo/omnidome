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

#include <omni/ui/AffineTransform.h>

#include "ui_omni_ui_AffineTransform.h"

namespace omni {
    namespace ui {

        AffineTransform::AffineTransform(QWidget* _parent) :
            QWidget(_parent),
            ui_(new Ui::AffineTransform)
        {
            ui_->setupUi(this);

            connect(ui_->btnRotate,SIGNAL(clicked()),this,SLOT(updateTransform()));
            connect(ui_->btnScale,SIGNAL(clicked()),this,SLOT(updateTransform()));
            connect(ui_->btnTranslate,SIGNAL(clicked()),this,SLOT(updateTransform()));

            connect(ui_->rotation,SIGNAL(xChanged()),this,SLOT(updateTransform()));
            connect(ui_->rotation,SIGNAL(yChanged()),this,SLOT(updateTransform()));
            connect(ui_->rotation,SIGNAL(zChanged()),this,SLOT(updateTransform()));

            for (auto& _scale : {ui_->scaleX,ui_->scaleY,ui_->scaleZ}) {
                connect(_scale,SIGNAL(valueChanged()),this,SLOT(updateTransform()));
                _scale->setRange(0.1,10.0);
                _scale->setPivot(1.0);
                _scale->setSingleStep(0.01);
                _scale->setPageStep(0.1);
            }

            for (auto& _offset : {ui_->offsetX,ui_->offsetY,ui_->offsetZ}) {

                _offset->setRange(-1.0,1.0);
                _offset->setPivot(0.0);
                _offset->setSingleStep(0.001);
                _offset->setPageStep(0.01);

                connect(_offset,SIGNAL(valueChanged()),this,SLOT(updateTransform()));
                registerScaledSlider(_offset);
            }
        }

        AffineTransform::~AffineTransform() {

        }

        omni::AffineTransform const* AffineTransform::transform() const {
            return transform_;
        }

        void AffineTransform::setTransform(omni::AffineTransform* _transform) {
            transform_ = _transform;

            this->locked([&]{
                ui_->btnRotate->setChecked(transform_->rotationEnabled());
                ui_->btnScale->setChecked(transform_->scaleEnabled());
                ui_->btnTranslate->setChecked(transform_->translationEnabled());

                ui_->rotation->setX(transform_->rotation().roll().degrees());
                ui_->rotation->setY(transform_->rotation().pitch().degrees());
                ui_->rotation->setZ(transform_->rotation().yaw().degrees());

                ui_->scaleX->setValue(transform_->scale().x());
                ui_->scaleY->setValue(transform_->scale().y());
                ui_->scaleZ->setValue(transform_->scale().z());

                ui_->offsetX->setValue(transform_->translation().x());
                ui_->offsetY->setValue(transform_->translation().y());
                ui_->offsetZ->setValue(transform_->translation().z());
            });

            updateTransform();
        }

        void AffineTransform::updateTransform() {
            if (!transform() || this->isLocked()) return;

            ui_->rotation->setVisible(ui_->btnRotate->isChecked());
            transform_->setRotationEnabled(ui_->btnRotate->isChecked());
            transform_->setRotation(
                EulerAngles(ui_->rotation->x(),ui_->rotation->y(),ui_->rotation->z()));

            ui_->scaleX->setVisible(ui_->btnScale->isChecked());
            ui_->scaleY->setVisible(ui_->btnScale->isChecked());
            ui_->scaleZ->setVisible(ui_->btnScale->isChecked());
            transform_->setScaleEnabled(ui_->btnScale->isChecked());
            transform_->setScale(QVector3D(
                ui_->scaleX->value(),ui_->scaleY->value(),ui_->scaleZ->value()));

            ui_->offsetX->setVisible(ui_->btnTranslate->isChecked());
            ui_->offsetY->setVisible(ui_->btnTranslate->isChecked());
            ui_->offsetZ->setVisible(ui_->btnTranslate->isChecked());
            transform_->setTranslationEnabled(ui_->btnTranslate->isChecked());
            transform_->setTranslation(QVector3D(
                ui_->offsetX->value(),ui_->offsetY->value(),ui_->offsetZ->value()));
        }

    }
}

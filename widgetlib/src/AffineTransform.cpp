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

#include <omni/ui/AffineTransform.h>

#include <QSignalBlocker>

#include "ui_omni_ui_AffineTransform.h"

namespace omni {
    namespace ui {
        AffineTransform::AffineTransform(QWidget *_parent) :
            QWidget(_parent),
            ui_(new Ui::AffineTransform)
        {
            setup();
        }

        AffineTransform::~AffineTransform() {}

        bool AffineTransform::isTranslationVisible() const {
            return ui_->btnTranslate->isVisible();
        }

        bool AffineTransform::isRotationVisible() const {
            return ui_->btnRotate->isVisible();
        }

        bool AffineTransform::isScaleVisible () const {
            return ui_->btnScale->isVisible();
        }

        float AffineTransform::scaleRange() const {
            return scaleRange_;
        }

        void AffineTransform::setTranslationVisible(bool _visible) {
            ui_->btnTranslate->setVisible(_visible);
            ui_->offsetX->setVisible(_visible);
            ui_->offsetY->setVisible(_visible);
            ui_->offsetZ->setVisible(_visible);
        }

        void AffineTransform::setRotationVisible(bool _visible) {
            ui_->btnRotate->setVisible(_visible);
            ui_->rotation->setVisible(_visible);
        }

        void AffineTransform::setScaleVisible(bool _visible) {
            ui_->btnScale->setVisible(_visible);
            ui_->scaleX->setVisible(_visible);
            ui_->scaleY->setVisible(_visible);
            ui_->scaleZ->setVisible(_visible);
        }

        void AffineTransform::setScaleRange(float _range) {
            if (_range <= 0.0) scaleRange_ = 1.0;

            scaleRange_ = _range;

            {
              QSignalBlocker blocker(this);
              for (auto& _scale : { ui_->scaleX, ui_->scaleY, ui_->scaleZ, ui_->scale }) {
                  _scale->setRange(1.0 / scaleRange_, scaleRange_);
                  _scale->setSingleStep(0.05 / scaleRange_);
                  _scale->setPageStep(0.5 / scaleRange_);
              }
            }
        }

        /// Set unit suffix for offset
        void AffineTransform::setOffsetUnit(QString const& _unit) {
          mixin_scale_type::setSuffix(_unit);
        }

        /// Set scale of ranges for offsets
        void AffineTransform::setOffsetRangeScale(float _scale) {
          mixin_scale_type::setScale(_scale);
          if (mixin_scale_type::rescaleValues()) {
            emit dataModelChanged();
          }
        }

        void AffineTransform::setRescaleOffsetValues(bool _rescaleValues) {
          mixin_scale_type::setRescaleValues(_rescaleValues);
        }

        void AffineTransform::dataToFrontend() {

            ui_->btnRotate->setChecked(dataModel()->rotationEnabled());
            ui_->btnScale->setChecked(dataModel()->scaleEnabled());
            ui_->btnTranslate->setChecked(dataModel()->translationEnabled());

            ui_->rotation->setVisible(ui_->btnRotate->isChecked());

            bool _uniformScale = dataModel()->uniformScaleEnabled();
            bool _scaleVisible = isScaleVisible() && ui_->btnScale->isChecked();

            ui_->chkUniformScale->setVisible(_scaleVisible);
            ui_->chkUniformScale->setChecked(_uniformScale);
            ui_->scale->setVisible(_scaleVisible && _uniformScale);
            ui_->scaleX->setVisible(_scaleVisible && !_uniformScale);
            ui_->scaleY->setVisible(_scaleVisible && !_uniformScale);
            ui_->scaleZ->setVisible(_scaleVisible && !_uniformScale);

            bool _offsetVisible = isTranslationVisible() && ui_->btnTranslate->isChecked();
            ui_->offsetX->setVisible(_offsetVisible);
            ui_->offsetY->setVisible(_offsetVisible);
            ui_->offsetZ->setVisible(_offsetVisible);

            ui_->rotation->setRotation(dataModel()->rotation());

            if (_uniformScale) {
              ui_->scale->setValue(dataModel()->uniformScale());
            } else {
              ui_->scaleX->setValue(dataModel()->scale().x());
              ui_->scaleY->setValue(dataModel()->scale().y());
              ui_->scaleZ->setValue(dataModel()->scale().z());
            }

            ui_->offsetX->setValue(dataModel()->translation().x());
            ui_->offsetY->setValue(dataModel()->translation().y());
            ui_->offsetZ->setValue(dataModel()->translation().z());
        }

        bool AffineTransform::frontendToData() {
            dataModel()->setRotationEnabled(ui_->btnRotate->isChecked());
            dataModel()->setRotation(
                EulerAngles(ui_->rotation->z(), ui_->rotation->y(),
                            ui_->rotation->x()));

            dataModel()->setScaleEnabled(ui_->btnScale->isChecked());
            dataModel()->setUniformScaleEnabled(ui_->chkUniformScale->isChecked());

            if (dataModel()->uniformScaleEnabled()) {
              dataModel()->setScale(ui_->scale->value());
            } else {
              dataModel()->setScale(QVector3D(
                                     ui_->scaleX->value(), ui_->scaleY->value(),
                                     ui_->scaleZ->value()));
            }

            dataModel()->setTranslationEnabled(ui_->btnTranslate->isChecked());
            dataModel()->setTranslation(QVector3D(
                                           ui_->offsetX->value(),
                                           ui_->offsetY->value(),
                                           ui_->offsetZ->value()));
            return true;
        }

        void AffineTransform::setup() {
            ui_->setupUi(this);

            connect(this, &AffineTransform::dataModelChanged,
                    this, &AffineTransform::updateFrontend);

            connect(ui_->btnRotate, &QToolButton::clicked,
                    this, &AffineTransform::updateDataModel);

            connect(ui_->btnScale, &QToolButton::clicked,
                    this, &AffineTransform::updateDataModel);

            connect(ui_->btnTranslate, &QToolButton::clicked,
                    this, &AffineTransform::updateDataModel);

            connect(ui_->rotation, &Rotation::rotationChanged,
                    this, &AffineTransform::updateDataModel);

            connect(ui_->chkUniformScale, &QCheckBox::clicked,
                    this, &AffineTransform::updateDataModel);

            for (auto& _scale : { ui_->scaleX, ui_->scaleY, ui_->scaleZ, ui_->scale }) {
                connect(_scale, &RangedFloat::valueChanged,
                        this, &AffineTransform::updateDataModel);
                _scale->setPivot(1.0);
                _scale->setDefaultValue(1.0);
                _scale->setUseDefaultValue(true);
            }
            setScaleRange(5.0);

            ui_->scale->setLabel("XYZ");
            ui_->scaleX->setLabel("X");
            ui_->scaleY->setLabel("Y");
            ui_->scaleZ->setLabel("Z");

            for (auto& _offset : { ui_->offsetX, ui_->offsetY, ui_->offsetZ }) {
                _offset->setRange(-1.0, 1.0);
                _offset->setPivot(0.0);
                _offset->setSingleStep(0.001);
                _offset->setPageStep(0.01);
                _offset->setDefaultValue(0.0);
                _offset->setUseDefaultValue(true);

                connect(_offset, &RangedFloat::valueChanged,
                        this, &AffineTransform::updateDataModel);
                registerScaledSlider(_offset);
            }

            ui_->offsetX->setLabel("X");
            ui_->offsetY->setLabel("Y");
            ui_->offsetZ->setLabel("Z");
        }
    }
}

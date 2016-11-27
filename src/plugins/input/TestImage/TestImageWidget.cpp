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

#include "TestImageWidget.h"

#include <QComboBox>

namespace omni {
    namespace ui {
        namespace input {
            TestImage::TestImage(omni::input::TestImage* _input,
                QWidget* _parent) :
                ParameterWidget(_parent),
                input_(_input) {
                setup();
            }

            TestImage::~TestImage() {

            }

            void TestImage::setMode(int _mode) {
                input_->setMode(
                  util::intToEnum<omni::input::TestImage::Mode>(_mode));
                preview_->update();
                emit inputChanged();
            }

            void TestImage::setFlipText(bool _flipText) {
                input_->setFlipText(_flipText);
                preview_->update();
                emit inputChanged();
            }

            void TestImage::setDisplayNumbers(bool _displayNumbers) {
                input_->setDisplayNumbers(_displayNumbers);
                preview_->update();
                emit inputChanged();
            }

            void TestImage::triggerUpdate() {
              preview_->triggerUpdate();
            }

            void TestImage::setup() {
                QLayout *_layout = new QVBoxLayout;
                _layout->setSpacing(2);
                _layout->setContentsMargins(0, 0, 0, 0);
                _layout->setSizeConstraint(QLayout::SetMaximumSize);
                setLayout(_layout);

                preview_.reset(new TestInputPreview(input_));
                _layout->addWidget(preview_.get());

                auto* _boxMode = new QComboBox();
                _boxMode->addItem("Fisheye");
                _boxMode->addItem("Equirectangular");
                _boxMode->addItem("CubeMap");
                connect(_boxMode,SIGNAL(currentIndexChanged(int)),this,SLOT(setMode(int)));
                _layout->addWidget(_boxMode);

                auto* _chkFlipText = addCheckBox("Flip text",input_->flipText());
                connect(_chkFlipText,SIGNAL(clicked(bool)),this,SLOT(setFlipText(bool)));
                auto* _chkDisplayNumbers = addCheckBox("Display numbers",input_->displayNumbers());
                connect(_chkDisplayNumbers,SIGNAL(clicked(bool)),this,SLOT(setDisplayNumbers(bool)));
                connect(preview_.get(),SIGNAL(inputChanged()),this,SIGNAL(inputChanged()));
            }
        }
    }
}

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

#include "CubeMapTestImageWidget.h"

namespace omni {
    namespace ui {
        namespace input {
            CubeMapTestImage::CubeMapTestImage(omni::input::CubeMapTestImage* _input,
                QWidget* _parent) :
                ParameterWidget(_parent),
                input_(_input) {
                setup();
            }

            CubeMapTestImage::~CubeMapTestImage() {

            }

            void CubeMapTestImage::setFlipText(bool _flipText) {
                input_->setFlipText(_flipText);
                preview_->update();
                emit inputChanged();
            }

            void CubeMapTestImage::setDisplayNumbers(bool _displayNumbers) {
                input_->setDisplayNumbers(_displayNumbers);
                preview_->update();
                emit inputChanged();
            }

            void CubeMapTestImage::triggerUpdate() {
              preview_->triggerUpdate();
            }

            void CubeMapTestImage::setup() {
                QLayout *_layout = new QVBoxLayout;
                _layout->setSpacing(2);
                _layout->setContentsMargins(0, 0, 0, 0);
                _layout->setSizeConstraint(QLayout::SetMaximumSize);
                setLayout(_layout);

                preview_.reset(new InputPreview(input_));
                _layout->addWidget(preview_.get());
                auto* _chkFlipText = addCheckBox("Flip text",input_->flipText());
                connect(_chkFlipText,SIGNAL(clicked(bool)),this,SLOT(setFlipText(bool)));
                auto* _chkDisplayNumbers = addCheckBox("Display numbers",input_->displayNumbers());
                connect(_chkDisplayNumbers,SIGNAL(clicked(bool)),this,SLOT(setDisplayNumbers(bool)));
                connect(preview_.get(),SIGNAL(inputChanged()),this,SIGNAL(inputChanged()));
            }
        }
    }
}

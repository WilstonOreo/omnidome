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

#include "Box.h"
#include "BoxWidget.h"

namespace omni {
    namespace ui {
        namespace canvas {
            Box::Box(QWidget *_parent) : CanvasParameters(_parent) {
            }

            Box::~Box() {}

            bool Box::frontendToData() {
                auto *_box = static_cast<omni::canvas::Box *>(dataModel()->canvas());
                _box->setSize(QVector3D(
                                  getParamAsFloat("Width"),
                                  getParamAsFloat("Length"),
                                  getParamAsFloat("Height")
                                  ));
                return CanvasParameters::frontendToData();
            }

            void Box::dataToFrontend() {
                    auto *_height = addOffsetWidget("Height", 0.5, 0.01, 1.0);
                    auto *_length = addOffsetWidget("Length", 0.5, 0.01, 1.0);
                    auto *_width = addOffsetWidget("Width", 0.5, 0.01, 1.0);

                    /// Retrieve parameters for Box canvas
                    auto *_box = static_cast<omni::canvas::Box *>(dataModel()->canvas());
                    _width->setValue(_box->size().x());
                    _length->setValue(_box->size().y());
                    _height->setValue(_box->size().z());

                    CanvasParameters::dataToFrontend();
            }
        }
    }
}

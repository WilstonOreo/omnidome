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

#include "Planar.h"
#include "PlanarWidget.h"

namespace omni {
    namespace ui {
        namespace canvas {
            Planar::Planar(QWidget *_parent) : CanvasParameters(_parent) {
                setup();
            }

            Planar::Planar(omni::canvas::Interface *_canvas, QWidget *_parent) :
                CanvasParameters(_canvas, _parent) {
                setup();
            }

            Planar::~Planar() {}

            void Planar::updateCanvasParameters() {
                auto* _planar = static_cast<omni::canvas::Planar*>(canvas());
                _planar->setHeight( getParamAsFloat("Length") );
                _planar->setWidth( getParamAsFloat("Width") );
            }

            void Planar::setup() {
                if (!canvas()) return;

                this->locked([&]() {
                    auto* _length = addOffsetWidget("Length",1.0,0.1,10.0);
                    auto* _width = addOffsetWidget("Width",1.0,0.1,10.0);

                    /// Retrieve parameters for Planar canvas
                    auto* _planar = static_cast<omni::canvas::Planar*>(canvas());
                    _width->setValue(_planar->width());
                    _length->setValue(_planar->height());
                });
            }
        }
    }
}

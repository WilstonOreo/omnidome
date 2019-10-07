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

#include "Cylinder.h"
#include "CylinderWidget.h"

namespace omni {
    namespace ui {
        namespace canvas {
            Cylinder::Cylinder(QWidget *_parent) : CanvasParameters(_parent) {
            }

            Cylinder::~Cylinder() {}

            bool Cylinder::frontendToData() {
                if (!dataModel()) return false;
                auto *_cylinder = static_cast<omni::canvas::Cylinder *>(dataModel()->canvas());
                _cylinder->setDiameter(getParamAsFloat("Diameter"));
                _cylinder->setHeight(getParamAsFloat("Height"));
                _cylinder->setBeginAngle(getParamAsFloat("Start angle"));
                _cylinder->setEndAngle(getParamAsFloat("End angle"));
                return CanvasParameters::frontendToData();
            }

            void Cylinder::dataToFrontend() {
              auto* _diameter = addOffsetWidget("Diameter",0.5,0.01,1.0);
              auto* _height = addOffsetWidget("Height",0.2,0.01,1.0);
              auto* _beginAngle = addAngleWidget("Start angle",0.0,0.0,360.0);
              auto* _endAngle = addAngleWidget("End angle",0.0,0.0,360.0);

                /// Retrieve parameters for Cylinder canvas
              auto* _cylinder = static_cast<omni::canvas::Cylinder*>(dataModel()->canvas());
              _diameter->setValue(_cylinder->diameter());
              _height->setValue(_cylinder->height());
              _beginAngle->setValue(_cylinder->beginAngle());
              _endAngle->setValue(_cylinder->endAngle());
              CanvasParameters::dataToFrontend();
            }
        }
    }
}

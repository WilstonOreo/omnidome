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

#include "HalfDome.h"
#include "HalfDomeWidget.h"

namespace omni {
    namespace ui {
        namespace canvas {
            HalfDome::HalfDome(QWidget *_parent) : CanvasParameters(_parent) {
            }

            HalfDome::~HalfDome() {}

            bool HalfDome::frontendToData() {
                if (!dataModel()) return false;
                auto *_halfdome = static_cast<omni::canvas::HalfDome *>(dataModel()->canvas());
                _halfdome->setDiameter(getParamAsFloat("Diameter"));
                return CanvasParameters::frontendToData();
            }

            void HalfDome::dataToFrontend() {
              auto* _diameter = addOffsetWidget("Diameter",0.5f,0.01f,1.0f);

                /// Retrieve parameters for HalfDome canvas
              auto* _halfdome = static_cast<omni::canvas::HalfDome*>(dataModel()->canvas());
              _diameter->setValue(_halfdome->diameter());
              CanvasParameters::dataToFrontend();
            }
        }
    }
}

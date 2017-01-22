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
#include "CylindricalWidget.h"
#include "Cylindrical.h"
#include <QVBoxLayout>

namespace omni {
    namespace ui {
        namespace mapping {
            Cylindrical::Cylindrical(QWidget *_widget) : ui::
                MappingParameters(_widget) {}

            Cylindrical::~Cylindrical() {}

            bool Cylindrical::frontendToData() {
                auto *_cylindrical =
                    static_cast<omni::mapping::Cylindrical *>(dataModel());

                _cylindrical->setBeginAngle(getParamAsFloat("Start"));
                _cylindrical->setEndAngle(getParamAsFloat("Stop"));
                return MappingParameters::frontendToData();
            }

            void Cylindrical::dataToFrontend() {
                // Set slider values for Cylindrical mapping
                auto *_cylindrical =
                    static_cast<omni::mapping::Cylindrical *>(dataModel());
                auto *_beginAngle = addOffsetWidget("Start",
                                                  _cylindrical->beginAngle(),
                                                  0.0,
                                                  1.0);

                _beginAngle->setSuffix("");
                auto *_endAngle = addOffsetWidget("Stop",
                                                     _cylindrical->endAngle(),
                                                     0.0,
                                                     1.0);
                _endAngle->setSuffix("");
                MappingParameters::dataToFrontend();
            }
        }
    }
}

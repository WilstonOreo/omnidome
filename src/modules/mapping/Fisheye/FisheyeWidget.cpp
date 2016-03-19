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
#include "FisheyeWidget.h"
#include "Fisheye.h"
#include <QVBoxLayout>

namespace omni {
    namespace ui {
        namespace mapping {
            Fisheye::Fisheye(QWidget* _widget) : ui::MappingParameters(_widget) {
                setup();
            }

            Fisheye::Fisheye(
                omni::mapping::Interface* _mapping,
                QWidget* _parent) :
                ui::MappingParameters(_mapping,_parent) {
                setup();
            }

            Fisheye::~Fisheye() {
            }

            void Fisheye::updateMappingParameters() {
                auto* _fisheye = static_cast<omni::mapping::Fisheye*>(mapping());
                _fisheye->setStretch(getParamAsFloat("Stretch"));
                _fisheye->setFlipHorizontal(getParamAsBool("Flip horizontal"));
                _fisheye->setFlipVertical(getParamAsBool("Flip vertical"));
            }

            void Fisheye::setup() {
                if (!mapping()) return;

                this->locked([&]() {
                // Set slider values for Fisheye mapping
                auto* _fisheye = static_cast<omni::mapping::Fisheye*>(mapping());
                auto* _stretch = addOffsetWidget("Stretch",0.0,0.0,1.0);
                _stretch->setValue(_fisheye->stretch());
                _stretch->setSuffix("");
                addDefaultParameters();
            });
            }

        }

    }
}

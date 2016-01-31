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
#include "PlanarWidget.h"
#include "Planar.h"
#include <QVBoxLayout>

namespace omni {
    namespace ui {
        namespace mapping {
            Planar::Planar(QWidget *_widget) : ui::
                MappingParameters(_widget) {
                setup();
            }

            Planar::Planar(
                omni::mapping::Interface *_mapping,
                QWidget                  *_parent) :
                ui::MappingParameters(_mapping, _parent) {
                setup();
            }

            Planar::~Planar() {}

            void Planar::updateMappingParameters() {
                auto* _planar = static_cast<omni::mapping::Planar*>(mapping());
                _planar->setOffset(QVector2D(getParamAsFloat("Offset X"),getParamAsFloat("Offset Y")));
                _planar->setStretch(QVector2D(getParamAsFloat("Stretch X"),getParamAsFloat("Stretch Y")));
                _planar->setFlipHorizontal(getParamAsBool(
                                                "Flip horizontal"));
                _planar->setFlipVertical(getParamAsBool("Flip vertical"));
            }

            void Planar::setup() {
                if (!mapping()) return;

                this->locked([&]() {
                    // Set slider values for Planar mapping
                    auto *_planar = static_cast<omni::mapping::Planar *>(mapping());
                    auto *_offsetX = addOffsetWidget("Offset X", 0.0, -1.0, 1.0);
                    auto *_offsetY = addOffsetWidget("Offset Y", 0.0, -1.0, 1.0);
                    auto *_stretchX = addOffsetWidget("Stretch X", 1.0, 0.0, 1.0);
                    auto *_stretchY = addOffsetWidget("Stretch Y", 1.0, 0.0, 1.0);
                    _offsetX->setValue(_planar->offset().x());
                    _offsetY->setValue(_planar->offset().y());
                    _stretchX->setValue(_planar->stretch().x());
                    _stretchY->setValue(_planar->stretch().y());
                    addFlipParameters();
                });
            }
        }
    }
}

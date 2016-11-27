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
#include "EquirectangularWidget.h"
#include "Equirectangular.h"
#include <QVBoxLayout>

namespace omni {
    namespace ui {
        namespace mapping {
            Equirectangular::Equirectangular(QWidget *_widget) : ui::
                MappingParameters(_widget) {}

            Equirectangular::~Equirectangular() {}

            bool Equirectangular::frontendToData() {
                auto *_equirectangular =
                    static_cast<omni::mapping::Equirectangular *>(dataModel());

                _equirectangular->setStripTop(getParamAsFloat("Strip Top"));
                _equirectangular->setStripBottom(getParamAsFloat("Strip Bottom"));
                return MappingParameters::frontendToData();
            }

            void Equirectangular::dataToFrontend() {
                // Set slider values for Equirectangular mapping
                auto *_equirectangular =
                    static_cast<omni::mapping::Equirectangular *>(dataModel());
                auto *_stripTop = addOffsetWidget("Strip Top",
                                                  _equirectangular->stripTop(),
                                                  0.0,
                                                  1.0);

                _stripTop->setSuffix("");
                auto *_stripBottom = addOffsetWidget("Strip Bottom",
                                                     _equirectangular->stripBottom(),
                                                     0.0,
                                                     1.0);
                _stripBottom->setSuffix("");
                MappingParameters::dataToFrontend();
            }
        }
    }
}

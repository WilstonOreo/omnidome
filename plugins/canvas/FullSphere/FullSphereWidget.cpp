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

#include "FullSphere.h"
#include "FullSphereWidget.h"

namespace omni {
  namespace ui {
    namespace canvas {
      FullSphere::FullSphere(QWidget *_parent) : CanvasParameters(_parent) {}

      FullSphere::~FullSphere() {}

      bool FullSphere::frontendToData() {
        auto *_fullsphere = static_cast<omni::canvas::FullSphere *>(dataModel()->canvas());

        _fullsphere->setDiameter(getParamAsFloat("Diameter"));
        return CanvasParameters::frontendToData();
      }

      void FullSphere::dataToFrontend() {
        auto* _diameter = addOffsetWidget("Diameter", 0.5f, 0.01f, 1.0f);

        /// Retrieve parameters for FullSphere canvas
        auto *_fullsphere = static_cast<omni::canvas::FullSphere *>(dataModel()->canvas());
        _diameter->setValue(_fullsphere->diameter());
        CanvasParameters::dataToFrontend();
      }
    }
  }
}

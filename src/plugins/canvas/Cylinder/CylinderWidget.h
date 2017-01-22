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

#ifndef OMNI_UI_CANVAS_CYLINDER_H_
#define OMNI_UI_CANVAS_CYLINDER_H_

#include <omni/ui/CanvasParameters.h>

namespace omni {
    namespace ui {
        namespace canvas {
            /**@brief Parameter widget for box canvas
             **/
            class Cylinder : public CanvasParameters {
                Q_OBJECT
            public:
                Cylinder(QWidget* _parent = nullptr);
                ~Cylinder();

            private:
                void dataToFrontend();

                /// Return true if data has changed by front end
                bool frontendToData();
            };
        }
    }
}

#endif /* OMNI_UI_CANVAS_CYLINDER_H_ */

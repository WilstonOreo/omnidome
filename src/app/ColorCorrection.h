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
#ifndef OMNI_UI_COLORCORRECTION_H_
#define OMNI_UI_COLORCORRECTION_H_

#include <memory>
#include <omni/ui/mixin/TuningWidget.h>
#include "DockWidget.h"

namespace omni {
    namespace ui {
        namespace Ui {
            class ColorCorrection;
        }

        class ColorCorrection :
            public DockWidget,
            public mixin::TuningWidget
        {
            Q_OBJECT
        public:
            ColorCorrection(QWidget* _parent = nullptr);
            ~ColorCorrection();

        public slots:
            void setChannel(int);

        signals:
            void colorCorrectionChanged();

        private:
            void tuningParameters();

            std::unique_ptr<Ui::ColorCorrection> ui_;
        };
    }
}

#endif /* OMNI_UI_COLORCORRECTION_H_ */

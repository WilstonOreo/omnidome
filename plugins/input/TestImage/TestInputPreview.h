/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_UI_TESTINPUTPREVIEW_H_
#define OMNI_UI_TESTINPUTPREVIEW_H_

#include "TestImage.h"
#include <omni/ui/InputPreview.h>

namespace omni {
    namespace ui {
        /// An input preview for display a test image input
        class TestInputPreview : public InputPreview {
            Q_OBJECT
        public:
            TestInputPreview(QWidget* = nullptr);
            TestInputPreview(input::TestImage*, QWidget* = nullptr);
            ~TestInputPreview();

        protected:
            void mouseMoveEvent(QMouseEvent *event);
            void mousePressEvent(QMouseEvent *event);
            void mouseReleaseEvent(QMouseEvent *event);

        private:
            /// Set ruler position of input from widget coordinates
            void setRulerPos(QPointF const&);
        };
    }
}

#endif /* OMNI_UI_TESTINPUTPREVIEW_H_ */

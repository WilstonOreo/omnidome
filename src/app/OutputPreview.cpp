/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#include "OutputPreview.h"

#include <QPainter>
#include <omni/Renderer.h>

namespace omni {
    namespace ui {
        OutputPreview::OutputPreview(QWidget* _parent) :
            QWidget(_parent) {

        }

        OutputPreview::~OutputPreview() {

        }

        RenderOptions const& OutputPreview::renderOptions() const {
            return renderOptions_;
        }

        void OutputPreview::setRenderOptions(RenderOptions const& _renderOptions) {
            renderOptions_ = _renderOptions;
            render();
        }

        void OutputPreview::render() {

            if (!session()) return;

            Renderer _renderer;

            std::map<QImage> _images;

            for (auto& _tuning : session_->tunings()) {
                auto _tuningPtr = _tuning.get();
                QImage _image;
                _renderer.render(_tuningPtr,_image);
            }

            switch(renderOptions_.separationMode()) {
                case SeparationMode::NONE:

                case SeparationMode::SCREENS:

                case SeparationMode::PROJECTORS:
            }

            update();
        }

        void OutputPreview::paintEvent(QPaintEvent* _event) {
            auto _rect = this->rect();

            QPainter _painter(this);

            QRect _imageRect(0,0,image_.width(),image_.height());

            auto _aspectRect = aspectRect(_rect,_imageRect);
            _painter.drawImage(_aspectRect,_image);

            auto _rect = getRectangles();

            for (auto& _rect : rectangles)



        }

        void OutputPreview::sessionParameters() {
            this->render();
        }


    }
}

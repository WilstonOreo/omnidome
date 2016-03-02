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
#include <QRectF>
#include <omni/Renderer.h>
#include <map>

namespace omni {
    namespace ui {
        OutputPreview::OutputPreview(QWidget* _parent) :
            QWidget(_parent),
            TransformedRect<OutputPreview>(this)
        {
        }

        OutputPreview::~OutputPreview() {

        }

        QRect OutputPreview::desktopRect() const {
            if (!session()) return QRect();
            QRect _rect = session()->screenSetup().combinedDesktopRect();
            switch(renderOptions_.mappingOutputMode()) {
                default:
                case mapping::OutputMode::MAPPED_INPUT:
                break;

                case mapping::OutputMode::TEXCOORDS:
                    _rect.setHeight(_rect.height() * 2);
                    break;
                case mapping::OutputMode::UVW:
                    _rect.setHeight(_rect.height() * 3);
                    break;
            }
            return _rect;
        }

        RenderOptions const& OutputPreview::renderOptions() const {
            return renderOptions_;
        }

        void OutputPreview::setRenderOptions(RenderOptions const& _renderOptions) {
            renderOptions_ = _renderOptions;
            render();
        }

        void OutputPreview::render() {
            return;
            if (!session()) return;

            Renderer _renderer(*session(),renderOptions_);

            QRectF _rect = this->transformedRect(session()->screenSetup().combinedDesktopRect());

            image_ = QImage(_rect.width(),_rect.height(),QImage::Format_ARGB32);

            std::map<omni::proj::Tuning const*,QImage> _images;

            for (auto& _tuning : session()->tunings()) {
                auto _tuningPtr = _tuning.get();

                QRectF _imageRect(session()->screenSetup().tuningRect(_tuningPtr));
                _imageRect = this->transformedRect(_imageRect);

                qDebug() << _tuningPtr->color() << _imageRect;

                QImage _image(_imageRect.width(),_imageRect.height(),QImage::Format_ARGB32);
                _renderer.render(_tuningPtr,_image);
                QPainter _painter(&image_);

                _painter.drawImage(_imageRect.x() - _rect.x(),_imageRect.y() - _rect.y(),_image);
            }

            switch(renderOptions_.separationMode()) {
                case SeparationMode::NONE:
                break;

                case SeparationMode::SCREENS:
                break;

                case SeparationMode::PROJECTORS:
                break;
            }

            update();
        }

        void OutputPreview::resizeEvent(QResizeEvent* _event) {
            render();
        }

        void OutputPreview::paintEvent(QPaintEvent* _event) {
            auto _rect = this->rect();

            QPainter _painter(this);
            _painter.setBrush(QBrush("#080808"));
            _painter.setPen(Qt::NoPen);
            _painter.drawRect(_rect);

            QRectF _imageRect(0,0,image_.width(),image_.height());
            qDebug() << "OutputPreview::paintEvent: " << _imageRect;

            //_painter.drawImage(_imageRect,image_);

            auto _screens = session()->screenSetup().screens();
            _screens.push_back(session()->screenSetup().standardScreen());

            _painter.setPen(QPen(QColor("#FFFFFF")));
            for (auto& _screen : _screens) {
                _painter.drawRect(transformedRect(_screen->geometry().translated(-desktopRect().topLeft())));
            }

                _painter.drawRect(transformedRect(session()->screenSetup().virtualDesktopRect().translated(-desktopRect().topLeft())));




//            for (auto& _rect : rectangles)

        }

        void OutputPreview::sessionParameters() {
            qDebug() << "OutputPreview::sessionParameters(): " << session();
//            this->render();
        }


    }
}

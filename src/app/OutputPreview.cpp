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
#include <omni/render/Renderer.h>
#include <map>

namespace omni {
    namespace ui {
        OutputPreview::OutputPreview(QWidget *_parent) :
            QWidget(_parent),
            TransformedRect<OutputPreview>(this)
        {}

        OutputPreview::~OutputPreview() {}

        QRect OutputPreview::desktopRect() const {
            if (!dataModel()) return QRect();

            using namespace render;
            auto& _exportSettings = dataModel()->exportSettings();

            QRect _rect = _exportSettings.excludeUnassignedProjectors()
                          ? dataModel()->screenSetup().desktopRect()
                          : dataModel()->screenSetup().combinedDesktopRect();

            if (_exportSettings.outputType() == OutputType::PLAIN_IMAGE) {
                _rect.setHeight(_rect.height() * verticalMultiplier());
            }

            return _rect;
        }

        void OutputPreview::render() {
            using namespace render;

            if (!dataModel()) return;

            return;

            QRectF _rect = this->transformedRect(
                dataModel()->screenSetup().combinedDesktopRect());

            image_ = QImage(_rect.width(), _rect.height(), QImage::Format_ARGB32);

            std::map<omni::proj::Tuning const *, QImage> _images;

            switch (dataModel()->exportSettings().separationMode()) {
            case SeparationMode::NONE:
                break;

            case SeparationMode::SCREENS:
                break;

            case SeparationMode::PROJECTORS:
                break;
            }
            update();
        }

        void OutputPreview::resizeEvent(QResizeEvent *_event) {
            render();
        }

        void OutputPreview::paintEvent(QPaintEvent *_event) {
            if (!dataModel()) return;

            auto& _screenSetup = dataModel()->screenSetup();
            auto& _exportSettings = dataModel()->exportSettings();

            auto _rect = this->rect();

            QPainter _painter(this);
            _painter.setBrush(QBrush("#080808"));
            _painter.setPen(Qt::NoPen);
            _painter.drawRect(_rect);

            QRectF _imageRect(0, 0, image_.width(), image_.height());

            // _painter.drawImage(_imageRect,image_);
            auto _screens = _screenSetup.screens(
                _exportSettings.excludeUnassignedProjectors());

            if (_screens.empty()) {
                QFont _font("Helvetica",30 / devicePixelRatio());
                _painter.setFont(_font);
                _painter.setPen(QPen("#ffffff"));
                _painter.setBrush(QBrush("#cccccc"));
                _painter.drawText(rect(),Qt::AlignCenter,"Nothing to export!");
                return;
            }


            for (auto& _screen : _screens) {
                drawScreen(_painter, _screen);
            }

            for (auto& _tuning :
                 _screenSetup.tunings(_exportSettings.
                                      excludeUnassignedProjectors()))
            {
                drawTuning(_painter, _tuning);
            }
        }

        void OutputPreview::drawTuning(QPainter          & _p,
                                       proj::Tuning const *_tuning) {
            auto & _screenSetup = dataModel()->screenSetup();
            QRectF _imageRect(_screenSetup.tuningRect(_tuning));

            render::Renderer _renderer(*dataModel());

            auto _tuningImageRect = this->transformedRect(_imageRect);

            QImage _image(_tuningImageRect.width(),
                          _tuningImageRect.height(), QImage::Format_ARGB32);
            _renderer.render(_tuning, _image);
            QPainter _painter(&image_);

            _p.drawImage(_tuningImageRect.x(),
                         _tuningImageRect.y(), _image);

            _p.setPen(QPen(_tuning->color(), 2));
            _p.setBrush(Qt::NoBrush);

            for (int i = 0; i < verticalMultiplier(); ++i) {
                _p.drawRect(transformedRect(_imageRect.translated(0,
                                                                  _imageRect.
                                                                  height() *
                                                                  i)).adjusted(1,
                1, -1, -1));
            }
        }

        void OutputPreview::drawScreen(QPainter& _p, QScreen const *_screen) {
            auto& _screenSetup = dataModel()->screenSetup();

            _p.setPen(QPen(QColor("#FFFFFF")));
            _p.setBrush(Qt::NoBrush);
            auto _rect = _screen ==
                         _screenSetup.standardScreen() ? _screenSetup.
                         virtualDesktopRect() :
                         _screen->geometry();
            _rect.translate(-desktopRect().topLeft());

            for (int i = 0; i < verticalMultiplier(); ++i) {
                _p.drawRect(transformedRect(_rect.translated(0,
                                                             _rect.height() *
                                                             i)));
            }
        }

        int OutputPreview::verticalMultiplier() const {
            auto& _exportSettings = dataModel()->exportSettings();

            using namespace render;

            if (_exportSettings.outputType() != OutputType::PLAIN_IMAGE) {
                return 1;
            }

            switch (_exportSettings.mappingOutputMode()) {
            case mapping::OutputMode::TEXCOORDS:
                return 2;

            case mapping::OutputMode::UVW:
                return 3;

            default:
                return 1;
            }
            return 1;
        }

        void OutputPreview::dataToFrontend() {}
    }
}

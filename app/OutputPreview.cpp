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

#include "OutputPreview.h"

#include <QPainter>
#include <QRectF>
#include <omni/render/Renderer.h>
#include <omni/proj/Calibration.h>
#include <map>

namespace omni {
    namespace ui {
        OutputPreview::OutputPreview(QWidget *_parent) :
            QWidget(_parent),
            TransformedRect<OutputPreview>(this)
        {
          startTimer(500.0);
        }

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

        void OutputPreview::triggerUpdate() {
          needsUpdate_ = true;
        }

        void OutputPreview::resizeEvent(QResizeEvent *_event) {
            triggerUpdate();
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
            auto _screens = _screenSetup.usedScreens();

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

            for (auto& _tuning : dataModel()->tunings()) {
	if (!_tuning->hasScreen() && _exportSettings.excludeUnassignedProjectors()) continue;

              drawTuning(_painter,_tuning.get());
            }

            needsUpdate_ = false;
        }

        void OutputPreview::timerEvent(QTimerEvent *) {
          if (needsUpdate_) {
            update();
          }
        }

        void OutputPreview::drawTuning(QPainter          & _p,
                                       proj::Tuning const *_tuning) {

            QRectF _imageRect(_tuning->contentGeometry().translated(-ScreenSetup::desktopRect().topLeft() + _tuning->screenGeometry().topLeft()));
            auto _tuningImageRect = this->transformedRect(_imageRect).toRect();

            auto& _exportSettings = dataModel()->exportSettings();

            proj::Calibration _calib;
            _calib.setRenderSize(_tuningImageRect.size());
            _calib.render(*_tuning);

            QImage _image;

            if (_exportSettings.outputType() == render::OutputType::PLAIN_IMAGE) {
              _image = _calib.toImage();
	 } else {
              _image = _calib.toPreviewImage();
            }

            _p.drawImage(_tuningImageRect.topLeft(), _image);
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
            auto _rect = _screenSetup.screenGeometry(_screen);
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

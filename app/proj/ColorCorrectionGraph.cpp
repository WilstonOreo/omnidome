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

#include "ColorCorrectionGraph.h"

#include <QBrush>
#include <QDebug>
#include <QPen>
#include <QRectF>
#include <QPainterPath>

namespace omni {
    namespace ui {
        namespace proj {
            ColorCorrectionGraph::ColorCorrectionGraph(QWidget* _parent) : QWidget(_parent) {

            }

            ColorCorrectionGraph::~ColorCorrectionGraph() {

            }



            void ColorCorrectionGraph::setChannel(Channel _channel) {
                channel_ = _channel;
            }

            Channel ColorCorrectionGraph::channel() const {
                return channel_;
            }

            void ColorCorrectionGraph::dataToFrontend() {
                update();
            }

            void ColorCorrectionGraph::paintEvent(QPaintEvent* _paintEvent) {
                QPainter _painter(this);
                _painter.setRenderHint(QPainter::HighQualityAntialiasing);
                _painter.setRenderHint(QPainter::Antialiasing, true);

                QRect _rect = rect();

                _painter.setBrush(QBrush("#212121"));
                _painter.setPen(Qt::NoPen);
                _painter.drawRect(_rect);
                drawGridLines(_painter);

                if (!isEnabled()) return;

                // Draw not selected graphs first
                drawGraphs(_painter,false);

                // Draw selected graphs after
                drawGraphs(_painter,true);
            }

            void ColorCorrectionGraph::resizeEvent(QResizeEvent* _resizeEvent) {
                update();
            }

            void ColorCorrectionGraph::drawGraphs(QPainter& _p, bool _selected) const {
                auto _drawGraph = [&](Channel _channel) {
                    if ((channel() == _channel) == _selected)
                        drawGraphForChannel(_p,_channel);
                };
                _drawGraph(Channel::ALL);
                _drawGraph(Channel::RED);
                _drawGraph(Channel::GREEN);
                _drawGraph(Channel::BLUE);
            }

            void ColorCorrectionGraph::drawGraphForChannel(
                QPainter& _p,
                omni::proj::ChannelCorrection const& _correction,
                QColor _color, bool _selected) const {

                QRectF _rect = QRectF(rect()).adjusted(2,2,-2,-2);

                _p.setBrush(Qt::NoBrush);


                QPen _pen(_color,_selected ? 1.5 : 1,Qt::SolidLine);
                _p.setPen(_pen);

                QPainterPath _path;
                for (double _posX = _rect.left(); _posX <= _rect.right(); _posX += 2) {
                    double _x = (_posX - _rect.left()) / double(_rect.width());
                    double _y = qBound(0.0,_correction.corrected(_x),1.0);
                    double _posY = float(_rect.height()) - _y*_rect.height() + _rect.top();
                    if (_posX == _rect.left()) {
                        _path.moveTo(_posX,_posY);
                    } else {
                        _path.lineTo(_posX,_posY);
                    }
                }

                _p.drawPath(_path);
            }

            void ColorCorrectionGraph::drawGraphForChannel(QPainter& _p, Channel _channel) const {
                if (!dataModel()) return;

                bool _selected = channel() == _channel;
                drawGraphForChannel(_p,*dataModel()->correction(_channel),
                    omni::proj::ColorCorrection::channelColor(_channel),_selected);
            }

            void ColorCorrectionGraph::drawGridLines(QPainter& _p) {
                QRectF _rect = QRectF(rect()).adjusted(2,2,-2,-2);
                QPen _pen("#080808");
                _p.setPen(_pen);
                // Draw vertical lines
                for (double _posX = _rect.left(); _posX <= _rect.right(); _posX += _rect.width()/8) {
                    _p.drawLine(_posX,_rect.top(),_posX,_rect.bottom());
                }

                // Draw horizontal lines
                for (double _posY = _rect.top(); _posY <= _rect.bottom(); _posY += _rect.height()/8) {
                    _p.drawLine(_rect.left(),_posY,_rect.right(),_posY);
                }
            }
        }
    }
}

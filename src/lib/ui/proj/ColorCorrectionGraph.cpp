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

#include <omni/ui/proj/ColorCorrectionGraph.h>

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

            void ColorCorrectionGraph::setSelectedChannel(Channel _selectedChannel) {
                selectedChannel_ = _selectedChannel;
            }

            Channel ColorCorrectionGraph::selectedChannel() const {
                return selectedChannel_;
            }

            void ColorCorrectionGraph::setColorCorrection(
                omni::proj::ColorCorrection const* _colorCorrection) {
                colorCorrection_ = _colorCorrection;
            }

            omni::proj::ColorCorrection const* ColorCorrectionGraph::colorCorrection() const {
                return colorCorrection_;
            }

            void ColorCorrectionGraph::paintEvent(QPaintEvent* _paintEvent) {
                QPainter _painter(this);
                _painter.setRenderHint(QPainter::HighQualityAntialiasing);
                _painter.setRenderHint(QPainter::Antialiasing, true);

                QRect _rect = rect();

                _painter.setBrush(QBrush("#212121"));
                _painter.setPen(Qt::NoPen);
                _painter.drawRect(_rect);

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
                    if ((selectedChannel() == _channel) == _selected)
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
                if (!colorCorrection_) return;

                bool _selected = selectedChannel() == _channel;
                switch (_channel) {
                    default:
                    case Channel::ALL:
                        drawGraphForChannel(_p,colorCorrection_->all(),Qt::white,_selected);
                        return;
                    case Channel::RED:
                        drawGraphForChannel(_p,colorCorrection_->red(),Qt::red,_selected);
                        return;
                    case Channel::GREEN:
                        drawGraphForChannel(_p,colorCorrection_->green(),Qt::green,_selected);
                        return;
                    case Channel::BLUE:
                        drawGraphForChannel(_p,colorCorrection_->blue(),Qt::blue,_selected);
                        return;
                }
            }
        }
    }
}

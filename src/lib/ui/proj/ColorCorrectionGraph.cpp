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
#include <QPen>

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

                QRect _rect = rect().adjusted(2,2,-2,-2);

                _p.setBrush(Qt::NoBrush);
                _p.setPen(QPen(_color,_selected ? 2 : 1));

                QVector<QPoint> _points;

                for (int _posX = _rect.left(); _posX <= _rect.right(); _posX += 2) {
                    float _x = (_posX - _rect.left()) / float(_rect.width());
                    float _y = qBound(0.0,_correction.corrected(_x),1.0);
                    int _posY = _rect.height() - _y*_rect.height() + _rect.top();
                    if (_points.size() > 0) _points << _points.back();
                    _points << QPoint(_posX,_posY);
                }
                _p.drawLines(_points);

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

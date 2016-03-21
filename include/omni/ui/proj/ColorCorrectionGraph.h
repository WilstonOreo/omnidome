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
#ifndef OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_
#define OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_

#include <QColor>
#include <QWidget>
#include <QPainter>
#include <omni/proj/ColorCorrection.h>
#include <omni/ui/mixin/DataModel.h>

namespace omni {
    namespace ui {
        namespace proj {
            using omni::proj::Channel;

            /// Widget which draws the graph for color corrections for each channel
            class ColorCorrectionGraph :
                public QWidget,
                public mixin::UnsharedDataModel<omni::proj::ColorCorrection> {
                Q_OBJECT
                OMNI_UI_UNSHARED_DATAMODEL(omni::proj::ColorCorrection)
            public:

                ColorCorrectionGraph(QWidget* = nullptr);
                ~ColorCorrectionGraph();

                Channel channel() const;
                void setChannel(Channel _channel);

            protected:
                void paintEvent(QPaintEvent*);
                void resizeEvent(QResizeEvent*);

            signals:
                void dataModelChanged();

            private:
                /// Update widget from current color correction
                void dataToFrontend();

                /// ColorCorrectionGraph cannot change data, hence there is nothing to do here
                inline bool frontendToData() { return false; }

                void drawGraphs(QPainter& _p, bool _selected) const;
                void drawGraphForChannel(QPainter&,
                    omni::proj::ChannelCorrection const&,
                    QColor _color, bool _selected) const;
                void drawGraphForChannel(QPainter&, Channel _channel) const;
                void drawGridLines(QPainter&);

                Channel channel_ = Channel::ALL;
            };
        }
    }
}

#endif /* OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_ */

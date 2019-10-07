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
#ifndef OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_
#define OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_

#include <QColor>
#include <QWidget>
#include <QPainter>
#include <omni/proj/ColorCorrection.h>
#include "mixin/DataModel.h"

namespace omni {
  namespace ui {
    namespace proj {
      using omni::proj::Channel;

      /// Widget which draws the graph for color corrections for each channel
      class ColorCorrectionGraph :
        public QWidget,
        public mixin::UnsharedDataModel<ColorCorrectionGraph,omni::proj::ColorCorrection>{
          Q_OBJECT
          OMNI_UI_UNSHARED_DATAMODEL(ColorCorrectionGraph,omni::proj::ColorCorrection)
        public:
          ColorCorrectionGraph(QWidget * = nullptr);
          ~ColorCorrectionGraph() override;

          /// Return selected channel
          Channel channel() const;

          /// Set selected channel
          void    setChannel(Channel _channel);

        protected:
          /// Paint graph event
          void    paintEvent(QPaintEvent *) override;

          /// Re-paint on resize
          void    resizeEvent(QResizeEvent *) override;

        signals:
          void    dataModelChanged();

        private:
          /// Update widget from current color correction
          void dataToFrontend() override;

          /**@brief ColorCorrectionGraph cannot change data, hence there is nothing to
                    do here
          **/
          inline bool frontendToData() override {
            return false;
          }

          /// Draw selected graphs
          void drawGraphs(QPainter& _p,
                          bool _selected) const;

          /// Draw graph for a channel correction
          void drawGraphForChannel(QPainter&,
                                   omni::proj::ChannelCorrection const&,
                                   QColor _color,
                                   bool _selected) const;

          /// Draw graph for a channel
          void drawGraphForChannel(QPainter&,
                                   Channel _channel) const;

          /// Draw grid lines in background
          void drawGridLines(QPainter&);

          Channel channel_ = Channel::ALL;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_COLORCORRECTIONGRAPH_H_ */

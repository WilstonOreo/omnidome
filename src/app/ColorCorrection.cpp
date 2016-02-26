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

#include "ColorCorrection.h"

#include <omni/util.h>
#include "ui_omni_ui_ColorCorrection.h"

namespace omni {
    namespace ui {
        ColorCorrection::ColorCorrection(QWidget* _parent) :
            DockWidget(_parent),
            ui_(new Ui::ColorCorrection)
        {
            this->setup(ui_);

            connect(ui_->boxChannel,SIGNAL(currentIndexChanged(int)),this,SLOT(setChannel(int)));
        }

        ColorCorrection::~ColorCorrection() {

        }

        void ColorCorrection::setChannel(int _index) {
            if (!tuning()) return;

            using proj::Channel;
            Channel _channel = util::intToEnum<Channel>(_index);
            auto* _colorCorrection = &tuning()->colorCorrection();
            ui_->graph->setColorCorrection(_colorCorrection);
            ui_->params->setChannelCorrection(_colorCorrection->correction(_channel));
        }

        void ColorCorrection::tuningParameters() {
            ui_->boxChannel->setCurrentIndex(0);
        }
    }
}

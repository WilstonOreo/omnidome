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
#ifndef OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_
#define OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_

#include <omni/proj/ColorCorrection.h>
#include <omni/proj/ChannelCorrection.h>
#include <omni/ui/ParameterWidget.h>

class QPushButton;

namespace omni {
    namespace ui {
        class RangedFloat;
        using omni::proj::ChannelCorrection;
        using omni::proj::Channel;

        namespace proj {
            class ChannelCorrectionParameters : public ParameterWidget
            {
                Q_OBJECT
            public:
                ChannelCorrectionParameters(QWidget* = nullptr);
                ~ChannelCorrectionParameters();

                void setChannelCorrection(ChannelCorrection*, Channel = Channel::ALL);
                ChannelCorrection* channelCorrection();
                ChannelCorrection const* channelCorrection() const;

            public slots:
                void updateParameters();
                void reset();

            private:
                void setup();

                ChannelCorrection* channelCorrection_ = nullptr;
                RangedFloat* brightness_ = nullptr;
                RangedFloat* contrast_ = nullptr;
                RangedFloat* gamma_ = nullptr;
                RangedFloat* multiplier_ = nullptr;
                QPushButton* reset_ = nullptr;
            };
        }
    }
}

#endif /* OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_ */

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

#include <omni/ui/proj/ChannelCorrectionParameters.h>

#include <QVBoxLayout>

namespace omni {
    namespace ui {
        namespace proj {
            ChannelCorrectionParameters::ChannelCorrectionParameters(QWidget* _parent) :
                ParameterWidget(_parent) {

                setup();
            }

            ChannelCorrectionParameters::~ChannelCorrectionParameters() {

            }
            void ChannelCorrectionParameters::setChannelCorrection(ChannelCorrection* _channel) {
                channelCorrection_ = _channel;
            }

            ChannelCorrection* ChannelCorrectionParameters::channelCorrection() {
                return channelCorrection_;
            }

            ChannelCorrection const* ChannelCorrectionParameters::channelCorrection() const {
                return channelCorrection_;
            }

            void ChannelCorrectionParameters::updateParameters() {
                if (!channelCorrection_) return;

                channelCorrection_->setBrightness(brightness_->value());
                channelCorrection_->setContrast(contrast_->value());
                channelCorrection_->setGamma(gamma_->value());
                emit parametersUpdated();
             }

            void ChannelCorrectionParameters::setup() {
                QLayout* _layout = new QVBoxLayout;
                setLayout(_layout);

                auto _addSlider = [&](QString const& _name) -> RangedFloat* {
                    auto* _slider = addWidget(_name,0.0,-1.0,1.0);
                    _slider->setSingleStep(0.01);
                    _slider->setPageStep(0.1);
                    _slider->setPivot(0.0);
                    connect(_slider,SIGNAL(valueChanged()),this,SLOT(updateParameters()));
                    return _slider;
                };
                
                brightness_ = _addSlider("Brightness");
                contrast_ = _addSlider("Contrast");
                gamma_ = _addSlider("Gamma");
            }
        }
    }
}

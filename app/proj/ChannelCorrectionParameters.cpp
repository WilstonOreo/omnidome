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

#include "proj/ChannelCorrectionParameters.h"

#include <QSignalBlocker>
#include <QPushButton>
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

            Channel ChannelCorrectionParameters::channel() const {
                return channel_;
            }

            void ChannelCorrectionParameters::setChannel(Channel _channel) {
                channel_ = _channel;
            }

            void ChannelCorrectionParameters::dataToFrontend() {
                QColor _color = omni::proj::ColorCorrection::channelColor(channel());
                auto _setSlider = [&](RangedFloat* _w, float _value) {
                    QColor _sliderColor = _color;
                    if (channel() == Channel::ALL) {
                        _sliderColor = QColor("#3d3d3d");
                    }
                    _w->setStyleSheet("selection-background-color  : "+_sliderColor.name());
                    _w->setValue(_value);
                    _w->setGripSize(0);
                };

                _setSlider(brightness_,dataModel()->brightness());
                _setSlider(contrast_,dataModel()->contrast());
                _setSlider(gamma_,dataModel()->gamma());
                _setSlider(multiplier_,dataModel()->multiplier());

                reset_->setStyleSheet("QPushButton { border: 2.5px solid " +
                    _color.name() + " }");
            }

            bool ChannelCorrectionParameters::frontendToData() {
                dataModel()->setBrightness(brightness_->value());
                dataModel()->setContrast(contrast_->value());
                dataModel()->setGamma(gamma_->value());
                dataModel()->setMultiplier(multiplier_->value());
                return true;
             }

            void ChannelCorrectionParameters::reset() {
                {
                  QSignalBlocker blocker(this);
                  brightness_->setValue(0.0);
                  contrast_->setValue(0.0);
                  gamma_->setValue(0.0);
                  multiplier_->setValue(0.5);
                }

                updateDataModel();
            }

            void ChannelCorrectionParameters::setup() {
                QLayout* _layout = new QVBoxLayout;
                setLayout(_layout);

                auto _addSlider = [&](QString const& _name) -> RangedFloat* {
                    auto* _slider = addWidget(_name,0.0,-1.0,1.0);
                    _slider->setSingleStep(0.001);
                    _slider->setPageStep(0.01);
                    _slider->setPivot(0.0);
                    connect(_slider,&RangedFloat::valueChanged,
                        this,&ChannelCorrectionParameters::updateDataModel);
                    return _slider;
                };

                {
                  QSignalBlocker blocked(this);
                  brightness_ = _addSlider("Brightness");
                  contrast_ = _addSlider("Contrast");
                  gamma_ = _addSlider("Gamma");
                  multiplier_ = _addSlider("Multiplier");
                  multiplier_->setRange(0.0,1.0);
                  multiplier_->setDefaultValue(0.5);

                  reset_ = new QPushButton("Reset");
                  layout()->addWidget(reset_);
                  connect(reset_,SIGNAL(clicked()),this,SLOT(reset()));
                }
            }
        }
    }
}

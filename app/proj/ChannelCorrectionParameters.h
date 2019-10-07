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
#ifndef OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_
#define OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_

#include <omni/proj/ColorCorrection.h>
#include <omni/proj/ChannelCorrection.h>
#include <omni/ui/ParameterWidget.h>
#include "mixin/DataModel.h"

class QPushButton;

namespace omni {
  namespace ui {
    class RangedFloat;
    using omni::proj::ChannelCorrection;
    using omni::proj::Channel;

    namespace proj {
      /// Parameter widget for channel correction
      class ChannelCorrectionParameters :
        public ParameterWidget,
        public mixin::UnsharedDataModel<ChannelCorrectionParameters,ChannelCorrection>{
          Q_OBJECT
          OMNI_UI_UNSHARED_DATAMODEL(ChannelCorrectionParameters,ChannelCorrection)
        public:
          ChannelCorrectionParameters(QWidget * = nullptr);
          ~ChannelCorrectionParameters() override;

          /// Return channel
          Channel channel() const;

          /// Set selected channel
          void    setChannel(Channel _channel);

        public slots:
          /// Reset color correction for channel
          void    reset();

        signals:
          /// Is emitted when channel correction has changed
          void    dataModelChanged();

        private:
          /// Setup widget
          void    setup();

          /// Update sliders from current warp grid
          void    dataToFrontend() override;

          /// Assign slider values to current warp grid
          bool    frontendToData() override;

          RangedFloat *brightness_ = nullptr;
          RangedFloat *contrast_   = nullptr;
          RangedFloat *gamma_      = nullptr;
          RangedFloat *multiplier_ = nullptr;
          QPushButton *reset_      = nullptr;
          Channel channel_         = Channel::ALL;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_CHANNELCORRECTIONPARAMETERS_H_ */

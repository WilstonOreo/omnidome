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
#ifndef OMNI_UI_COLORCORRECTION_H_
#define OMNI_UI_COLORCORRECTION_H_

#include <omni/proj/Channel.h>
#include "mixin/DataModel.h"
#include "DockWidget.h"

namespace omni {
  class Session;

  namespace ui {
    namespace Ui {
      class ColorCorrection;
    }

    /// Dockwidget for editing color correction of a projector
    class ColorCorrection :
      public DockWidget,
      public mixin::SharedDataModel<ColorCorrection,Session>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(ColorCorrection,Session)
      public:
        typedef omni::proj::Channel Channel;

        ColorCorrection(QWidget *_parent = nullptr);
        ~ColorCorrection() override;

      public slots:
        /// Select color correction for all channels
        void setAllChannels();

        /// Select color correction for red channel
        void setRedChannel();

        /// Select color correction for green channel
        void setGreenChannel();

        /// Select color correction for blue channel
        void setBlueChannel();

        /// Select color correction for custom channel
        void setChannel(Channel _channel);

        /// Select whether color correction is used
        void setUsed(bool);

      signals:
        void dataModelChanged();

      private:
        /// Update sliders from current color correction
        void dataToFrontend() override;

        /// Assign slider values to current color correction
        bool frontendToData() override;

        std::unique_ptr<Ui::ColorCorrection> ui_;
    };
  }
}

#endif /* OMNI_UI_COLORCORRECTION_H_ */

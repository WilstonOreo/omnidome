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

#pragma once

#include <omni/visual/ContextBoundPtr.h>
#include <omni/input/Framebuffer.h>
#include <SpoutReceiver.h>

namespace omni {
  namespace input {
    class Spout : public QObject, public Framebuffer {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID)
        Q_INTERFACES(omni::input::Interface)
        OMNI_PLUGIN_INFO("Spout Receiver", "Copyright (C) 2019")
      public:
        OMNI_REGISTER_CLASS(Factory, Spout)

        /// Default Constructor
        Spout();
        ~Spout() final;

        /// Install update timer on activate
        void     activate() override;

        /// Remove update timer on deactivate
        void     deactivate() override;

        /// Update image
        void     update() override;

        /// Update image and trigger update callback signal
        void     callUpdated();

      protected:
        void timerEvent(QTimerEvent*) override;

      private:
        void initTexture();

        SpoutReceiver spoutReceiver_;
        GLuint textureId_ = 0;
        int timerId_ = 0;
        char senderName_[256];
    };
  }
}

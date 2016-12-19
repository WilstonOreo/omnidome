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
#ifndef OMNI_INPUT_BLACKMAGIC_H_
#define OMNI_INPUT_BLACKMAGIC_H_

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <omni/visual/ContextBoundPtr.h>
#include <omni/input/Framebuffer.h>

class IDeckLink;
class IDeckLinkDisplayMode;

namespace omni {
  namespace input { 
    class BlackmagicCapture;

    class Blackmagic : public QObject, public Framebuffer {
      OMNI_INPUT_PLUGIN_DECL
      OMNI_PLUGIN_INFO("Blackmagic DeckLink Input","Copyright (C) 2016")
      public:
        OMNI_REGISTER_CLASS(Factory, Blackmagic)

        Blackmagic();
        ~Blackmagic();

        /// Activate input and stop capturing
        void activate();

        /// Deactivate input and stop capturing
        void deactivate();

        /// Capture frame and write to GPU memory
        void update();

        /// Serialize image path to property map
        void     toPropertyMap(PropertyMap&) const;

        /// Deserialize from property map and load image
        void     fromPropertyMap(PropertyMap const&);

        /// Return id of currently used device
        int       deviceId() const;

        /// Set new device id, input is reactivated
        void      setDeviceId(int);

        void      setup(IDeckLink*,IDeckLinkDisplayMode*);

        QWidget*  widget();

      private:
        static ContextBoundPtr<QOpenGLShaderProgram> shader_;

        int deviceId_ = 0;
        bool isCapturing_ = false;
        QSize size_;
        float framerate_ = 0.0;
        BlackmagicCapture* capture_ = nullptr;
    };
  }
}

#endif /* OMNI_INPUT_BLACKMAGIC_H_ */


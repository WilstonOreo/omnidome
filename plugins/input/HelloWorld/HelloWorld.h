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

#ifndef OMNI_INPUT_HELLOWORLD_H_
#define OMNI_INPUT_HELLOWORLD_H_

#include <QOpenGLShaderProgram>
#include <omni/visual/ContextBoundPtr.h>
#include <omni/input/Framebuffer.h>

namespace omni {
  namespace input {
    class HelloWorld : public QObject, public Framebuffer {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID)
        Q_INTERFACES(omni::input::Interface)
        OMNI_PLUGIN_INFO("Hello World", "Copyright (C) 2016")
      public:
        OMNI_REGISTER_CLASS(Factory, HelloWorld)

        /// Default Constructor
        HelloWorld();
        ~HelloWorld() final = default;

        /// Install update timer on activate
        void     activate() override;

        /// Remove update timer on deactivate
        void     deactivate() override;

        /// Update image
        void     update() override;

        /// Update image and trigger update callback signal
        void     callUpdated();

        /// Return image dimensions in pixels
        QSize    size() const override;

      protected:
        void timerEvent(QTimerEvent*) override;

      private:
        ContextBoundPtr<QOpenGLShaderProgram> shader_;
        double  startTime_;
        int timerId_ = 0;
    };
  }
}

#endif /* OMNI_INPUT_HELLOWORLD_H_ */

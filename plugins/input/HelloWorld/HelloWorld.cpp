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

#include "HelloWorld.h"

#include <omni/visual/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>
#include <omni/util.h>
#include <chrono>

namespace omni {
  namespace input {
    /// Default Constructor
    HelloWorld::HelloWorld() {
      startTime_ = visual::util::now();
    }

    void HelloWorld::timerEvent(QTimerEvent *) {
      this->update();
      this->triggerUpdateCallbacks();
    }

    /// Install update timer on activate
    void     HelloWorld::activate() {
      int _timerId = this->startTimer(40);
      if (!timerId_) {
        timerId_ = _timerId;
      }
      this->update();
    }

    /// Remove update timer on deactivate
    void      HelloWorld::deactivate() {
      this->killTimer(timerId_);
      timerId_ = 0;
    }

    /// Update image
    void     HelloWorld::update() {
      if (!QOpenGLContext::currentContext()) return;

      using namespace visual;
      double _time = visual::util::now() - startTime_;

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        setupFramebuffer(size());
        initShader(shader_,"shader");
      });

      visual::draw_on_framebuffer(framebuffer(),
                                  [&](QOpenGLFunctions& _) // Projection
                                                           // Operation
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      },
                                  [&](QOpenGLFunctions& _) // Model View
                                                           // Operation
      {
        useShader(*shader_,[&](UniformHandler& _handler) {
          _handler.uniform("iGlobalTime",GLfloat(_time));
          _handler.uniform("iResolution",QVector2D(size().width(), size().height()));
          visual::Rectangle::draw();

        });
      });
    }

    QSize    HelloWorld::size() const {
      return QSize(2048, 2048);
    }
  }
}

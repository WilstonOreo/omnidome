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

#include "Spout.h"

#include <omni/visual/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>
#include <omni/util.h>

#include <QOpenGLTexture>
#include <QDebug>

namespace omni {
  namespace input {
    /// Default Constructor
    Spout::Spout() {
        senderName_[0] = 0; // the name will be filled when the receiver connects to a sender
    }

    Spout::~Spout()
    {
        withCurrentContext([&](QOpenGLFunctions& _) {
            if(textureId_ != 0) {
                _.glDeleteTextures(1, &textureId_);
                textureId_ = 0;
            }
        });
    }

    /// Install update timer on activate
    void     Spout::activate() {

        this->update();
        int _timerId = this->startTimer(spoutReceiver_.GetVerticalSync());
        if (!timerId_) {
          timerId_ = _timerId;
        }
        this->update();
    }

    /// Remove update timer on deactivate
    void      Spout::deactivate() {
        if (!framebuffer())
            spoutReceiver_.ReleaseReceiver();

        this->killTimer(timerId_);
        timerId_ = 0;
    }

    /// Update image
    void     Spout::update() {
      if (!QOpenGLContext::currentContext()) return;

      unsigned int w = 0, h = 0;

      if (!framebuffer()) {
          if (spoutReceiver_.CreateReceiver(senderName_, w, h, true)) {
              if (int(w) != size().width() || int(h) != size().height()) {
                  setupFramebuffer(QSize(int(w),int(h)));
                  initTexture();
              }
          }
      }

      if (framebuffer()) {
          if (spoutReceiver_.ReceiveTexture(senderName_,w,h, textureId_, GL_TEXTURE_2D, false, framebuffer()->texture())) {
              if (int(w) != size().width() || int(h) != size().height()) {
                  setSize(QSize(int(w),int(h)));
              }

              visual::draw_on_framebuffer(framebuffer(),[&](QOpenGLFunctions&) {
                  spoutReceiver_.DrawSharedTexture();
              });

          } else {
              spoutReceiver_.ReleaseReceiver();
              destroy();
          }
      }
    }

    void Spout::timerEvent(QTimerEvent *)
    {
        update();
        this->triggerUpdateCallbacks();
    }

    void Spout::initTexture()
    {
        withCurrentContext([&](QOpenGLFunctions& _) {
            if(textureId_ != 0) {
                _.glDeleteTextures(1, &textureId_);
                textureId_ = 0;
            }

            _.glGenTextures(1, &textureId_);
            _.glBindTexture(GL_TEXTURE_2D, textureId_);
            _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            _.glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            _.glBindTexture(GL_TEXTURE_2D, 0);
        });
    }

  }
}

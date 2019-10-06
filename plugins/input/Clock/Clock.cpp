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

#include "Clock.h"

#include <omni/visual/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>
#include <omni/util.h>
#include <chrono>
#include <QDateTime>
#include "ClockWidget.h"

namespace omni {
  namespace input {
    /// Default Constructor
    Clock::Clock() {
        setCountDown(0,30,0);
        startTime_ = visual::util::now();
    }

    void Clock::timerEvent(QTimerEvent *) {
      if (!countDownPaused_) {
        currentCountDownTime_ -= 1 / 10.0;
      }

      this->update();
      this->triggerUpdateCallbacks();
    }

    QTime  Clock::countdownTime() const {
      double _countdown = std::max(currentCountDownTime_,0.0);

      return QTime(
        int(_countdown / 60.0 / 60.0) % 100, // Hours
        int(_countdown / 60.0) % 60, // Minutes
        int(_countdown) % 60); // Seconds

    }

    double Clock::now() const {
      QDateTime _dateTime = QDateTime::currentDateTime();
      double _time = _dateTime.time().hour() * 60.0 * 60.0;
      _time += _dateTime.time().minute() * 60.0;
      _time += _dateTime.time().second();
      return _time;
    }

    double Clock::getTime() const {
      double _countdown = std::max(currentCountDownTime_,0.0);
      return useSystemTime_ ?
        now() : _countdown;
    }

    int Clock::getMinutes() const {
      return int(getTime() / 60.0) % 60;
    }

    int Clock::getHours() const {
      return int(getTime() / 60.0 / 60.0) % 100;
    }

    int Clock::getSeconds() const {
      return int(getTime()) % 60;
    }

    void Clock::resetCountDown() {
      currentCountDownTime_ = countdownTime_;
    }

    void Clock::setCountDown(int _hour, int _min, int _sec) {
      countdownTime_ = _sec + 60.0 * _min + 60.0 * 60.0 * _hour;
      if (currentCountDownTime_ <= 0.0) {
        resetCountDown();
      }
    }

    bool Clock::useSystemTime() const {
      return useSystemTime_;
    }

    void Clock::setUseSystemTime(bool _useSystemTime) {
      useSystemTime_ = _useSystemTime;
    }

    void Clock::pauseCountDown(bool _paused) {
      countDownPaused_ = _paused;
    }

    bool Clock::countDownPaused() const {
      return countDownPaused_;
    }

    /// Install update timer on activate
    void     Clock::activate() {
      int _timerId = this->startTimer(100);
      if (!timerId_) {
        timerId_ = _timerId;
      }
      this->update();
    }

    /// Remove update timer on deactivate
    void      Clock::deactivate() {
      this->killTimer(timerId_);
      timerId_ = 0;
    }

    /// Update image
    void     Clock::update() {
      if (!QOpenGLContext::currentContext()) return;

      using namespace visual;

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        setupFramebuffer(size());
        initShader(shader_,"ClockShader");
      });

      draw_on_framebuffer(framebuffer(),
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
        useShader(*shader_,[&](UniformHandler& _h) {
          _h.uniform("t",GLfloat(visual::util::now() - startTime_));
          _h.uniform("hours", GLfloat(getHours()));
          _h.uniform("minutes", GLfloat(getMinutes()));
          _h.uniform("seconds", GLfloat(getSeconds()));
          _h.uniform("blinking", currentCountDownTime_ < 0);
          Rectangle::drawFlipped();
        });
      });
    }

    QWidget* Clock::widget() {
      return new omni::ui::input::Clock(this);
    }

    QSize    Clock::size() const {
      return QSize(1024, 256);
    }

    void     Clock::toPropertyMap(PropertyMap& _map) const {
      input::Interface::toPropertyMap(_map);
    }

    void     Clock::fromPropertyMap(PropertyMap const& _map) {
      input::Interface::fromPropertyMap(_map);
    }
  }
}

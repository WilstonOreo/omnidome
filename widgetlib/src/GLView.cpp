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

#include <omni/ui/GLView.h>

#include <omni/Session.h>
#include <omni/util.h>
#include <omni/visual/util.h>

#include <QMouseEvent>
#include <chrono>

namespace omni
{
  using visual::util::now;

  namespace ui
  {
    GLView::GLView(QWidget* _parent) :
      QOpenGLWidget(_parent)
    {
    }

    GLView::~GLView()
    {
      // Kill timer
      setUpdateFrequency(0.0);
    }

    void GLView::triggerUpdate() {
      if (updateFreq_ <= 0.0) update();

      updateTriggered_ = false;
    }

    void GLView::timerEvent(QTimerEvent*) {
      if (!updateTriggered_) {
          update();
      }
    }

    float GLView::updateFrequency() const {
      return updateFreq_;
    }

    void GLView::setUpdateFrequency(float _updateFrequency) {
      updateFreq_ = _updateFrequency;

      if (timerId_) {
        killTimer(timerId_);
        timerId_ = 0;
      }

      if (updateFreq_ <= 0.0) {
        return;
      }

      int _timerId = startTimer(1000.0 / updateFreq_);
      if (!timerId_) {
        timerId_ = _timerId;
      }
    }

    float GLView::aspect() const
    {
      return (float)width() / height();
    }

    QPointF GLView::mousePosition() const
    {
      return mousePosition_;
    }

    bool GLView::initialized() const
    {
      return initialized_;
    }

    void GLView::initializeGL()
    {
      initializeOpenGLFunctions();

      makeCurrent();
      visual::resetOpenGLState();

      initialized_ = initialize();
    }

    void GLView::resizeGL(int _w, int _h)
    {
      _w = _w & ~1;
      _h = _h & ~1;
      makeCurrent();
      visual::viewport(this);
      withCurrentContext([&](QOpenGLFunctions& _) {
        _.glClearColor(0.0,0.0,0.0,1.0);
      });

      update();
    }

    void GLView::paintGL()
    {
      double _seconds = util::benchmark([&]() {
        paint(); 
        updateTriggered_ = true;
      });

      currentFps_ = 1.0 / _seconds;
    }

    void GLView::mousePressEvent(QMouseEvent* event)
    {
      QOpenGLWidget::mousePressEvent(event);
      setFocus();
      mousePosition_ = event->pos();

      update();
    }
  }
}

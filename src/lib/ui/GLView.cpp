/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
    }

    void GLView::updateWithFrameRate() {
//      if (canDraw()) {.DS_Stor
      if (updateFreq_ <= 0.0) update();
//      }

      updateTriggered_ = false;
    }

    void GLView::timerEvent(QTimerEvent*) {
      if (!updateTriggered_) {
          update();
      }
      updateTriggered_ = true;
    }

    bool GLView::canDraw() const {
      bool _needsUpdate = (updateFreq_ <= 0.0) || ((now() - lastUpdateTime_) >= 1.0 / updateFreq_);
      return _needsUpdate;
    }

    float GLView::updateFrequency() const {
      return updateFreq_;
    }

    void GLView::setUpdateFrequency(float _updateFrequency) {
      updateFreq_ = _updateFrequency;
      if (updateFreq_ <= 0.0) {
        if (timerId_) {
          killTimer(timerId_);
        }
        return;
      }

      int _timerId = startTimer(1000.0 / updateFreq_);
      if (!timerId_) {
        timerId_ = _timerId;
      }
    }

    void GLView::paintGLReady() {
      lastUpdateTime_ = now();
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

      visual::with_current_context([this](QOpenGLFunctions& _) {
        _.glEnable(GL_DEPTH_TEST);
        _.glDepthFunc(GL_LEQUAL);
        _.glEnable(GL_BLEND);
        _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        _.glEnable(GL_LINE_SMOOTH);
        _.glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        _.glEnable(GL_POINT_SMOOTH);
        _.glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        _.glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glPolygonMode(GL_FRONT,GL_FILL);
        glPolygonMode(GL_BACK,GL_FILL);
        _.glEnable(GL_NORMALIZE);
        // fix outlines z-fighting with quads
        _.glPolygonOffset(1, 1);
        setAutoFillBackground(true);
      });


      initialized_ = initialize();
    }

    void GLView::resizeGL(int _w, int _h)
    {
       if (this->isLocked()) return;

      _w = _w & ~1;
      _h = _h & ~1;
      makeCurrent();
      visual::viewport(this);
      visual::with_current_context([&](QOpenGLFunctions& _) {
        _.glClearColor(0.0,0.0,0.0,1.0);
      });

      update();
    }

    void GLView::paintGL()
    {
    }

    void GLView::dataToFrontend()
    {
      vizSession_.reset(new visual::Session(*dataModel()));
      initialized_ = initialize();
    }

    void GLView::mousePressEvent(QMouseEvent* event)
    {
      QOpenGLWidget::mousePressEvent(event);
      setFocus();
      if (!vizSession_) return;
      mousePosition_ = event->pos();

      update();
    }
  }
}

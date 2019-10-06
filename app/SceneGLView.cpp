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

#include "SceneGLView.h"

#include <QMouseEvent>
#include <omni/util.h>
#include <omni/visual/util.h>

namespace omni {
  namespace ui {
    SceneGLView::SceneGLView(QWidget *_parent) :
      GLView(_parent)
    {}

    SceneGLView::~SceneGLView()
    {}

    bool SceneGLView::initialize()
    {
      dataModel()->makeVisualizer();
      dataModel()->visualizer()->update();

      if (dataModel()->canvas()) {
        dataModel()->canvas()->update();
      }

      dataModel()->scene().updateGrid();
      dataModel()->scene().updateLights();
      return true;
    }

    void SceneGLView::showEvent(QShowEvent *event) {
      makeCurrent();
      dataModel()->makeVisualizer();
      dataModel()->visualizer()->update();

      if (dataModel()->canvas()) {
        dataModel()->canvas()->update();
      }

      dataModel()->scene().updateGrid();
      dataModel()->scene().updateLights();
    }

    void SceneGLView::paint()
    {
      if (!dataModel()) return;

      auto *_vizSession = dataModel()->visualizer();
      if (!_vizSession) return;

      auto& _scene = dataModel()->scene();

      withCurrentContext([&](QOpenGLFunctions& _) {
        _.glClear(
          GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        visual::viewport(this);

        _vizSession->update();

        _scene.camera()->setup(aspect());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        _scene.updateLights();

        _.glEnable(GL_BLEND);
        _.glEnable(GL_DEPTH_TEST);
        _vizSession->drawCanvasFor3DView();

        _vizSession->drawProjectors(_scene.displayProjectors());
        _vizSession->drawCanvasWithFrustumIntersections(_scene.
                                                        displayProjectedAreas());
        _vizSession->drawProjectorHalos(_scene.displayProjectors());

        /// Draw auxiliary elements of canvas, like bounding boxes etc

        dataModel()->canvas()->drawAux();

        _scene.drawGrid();
      });
    }

    void SceneGLView::wheelEvent(QWheelEvent *event)
    {
      if (!dataModel()) return;

      auto *_cam = dataModel()->scene().camera();

      if (!_cam) return;

      float _r = event->delta() / 100.0;
      _cam->track(0, 0, _r);

      auto _size = dataModel()->scene().size();
      _cam->limitDistance(_size * 0.1, _size * 5.0);
      triggerUpdate();
    }

    void SceneGLView::keyPressEvent(QKeyEvent *event)
    {}

    void SceneGLView::mouseMoveEvent(QMouseEvent *event)
    {
      if (!dataModel()) return;

      auto *_cam = dataModel()->scene().camera();

      if ((event->buttons() & Qt::LeftButton) && _cam)
      {
        if (event->modifiers() & Qt::ShiftModifier)
        {
          _cam->strafe((event->pos().x() - mousePosition().x()) / 20.0);
          _cam->lift((event->pos().y() - mousePosition().y()) / 20.0);
        }
        else
        {
          if (!(event->modifiers() & Qt::ControlModifier)) _cam->track(
              event->pos().x() - mousePosition().x(),
              event->pos().y() - mousePosition().y(), 0);
        }
      }

      this->mousePosition_ = event->pos();
      triggerUpdate();
    }

    void SceneGLView::changeZoom(int _value)
    {
      if (!dataModel()) return;

      auto *_cam = dataModel()->scene().camera();

      if (!_cam) return;

      _cam->setDistance(_value / 5.0);
      triggerUpdate();
    }

    void SceneGLView::dataToFrontend()
    {
      makeCurrent();
    }
  }
}

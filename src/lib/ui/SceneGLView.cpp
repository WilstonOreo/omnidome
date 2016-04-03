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

#include <omni/ui/SceneGLView.h>

#include <QMouseEvent>
#include <omni/util.h>
#include <omni/visual/util.h>

namespace omni
{
  namespace ui
  {
    SceneGLView::SceneGLView(QWidget* _parent) :
      GLView(_parent)
    {
    }

    SceneGLView::~SceneGLView()
    {
    }

    bool SceneGLView::initialize()
    {
      if (!dataModel() || initialized() || !context()) return false;

      dataModel()->scene().updateLights();
      grid_.reset(new visual::Grid(dataModel()->scene().camera()));
      grid_->update();

      if (dataModel()->canvas()) {
        dataModel()->canvas()->update();
      }

      vizSession_->update();
      return true;
    }


    void SceneGLView::showEvent(QShowEvent* event) {
        if (vizSession_) {
          vizSession_->update();
        }
    }

    void SceneGLView::paintGL()
    {
      if (!dataModel() || this->isLocked() || !initialized()) return;

      auto& _scene = dataModel()->scene();

      glEnable(GL_DEPTH_TEST);

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      visual::viewport(this);

      vizSession_->update();

      _scene.camera()->setup(aspect());

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      _scene.updateLights();

      vizSession_->drawCanvas(_scene.displayInput() && dataModel()->hasOutput() ?
          mapping::OutputMode::MAPPED_INPUT : mapping::OutputMode::LIGHTING_ONLY);

      vizSession_->drawProjectors(!_scene.displayProjectors());
      vizSession_->drawCanvasWithFrustumIntersections(_scene.projectorViewMode(),!_scene.displayProjectedAreas());
      vizSession_->drawProjectorHalos(!_scene.displayProjectors());

      if (_scene.displayGrid()) {
        grid_->draw(0.5);
        glDisable(GL_DEPTH_TEST);
        grid_->draw(0.5);
      }
    }

    void SceneGLView::wheelEvent(QWheelEvent* event)
    {
      if (!dataModel()) return;

      auto* _cam = dataModel()->scene().camera();
      if (!_cam) return;

      float _r = event->delta()/100.0;
      _cam->track( 0, 0, _r );

      auto _size = dataModel()->scene().scale();
      _cam->limitDistance(_size*0.1,_size*5.0);
      triggerUpdate();
    }

    void SceneGLView::keyPressEvent(QKeyEvent* event)
    {
    }

    void SceneGLView::mouseMoveEvent(QMouseEvent *event)
    {
      if (!dataModel()) return;

      auto* _cam = dataModel()->scene().camera();

      if ((event->buttons() & Qt::LeftButton) && _cam)
      {
        if( event->modifiers() & Qt::ShiftModifier )
        {
          _cam->strafe((event->pos().x() - mousePosition().x())/20.0);
          _cam->lift((event->pos().y() - mousePosition().y())/20.0);
        }
        else
        {
          if( !(event->modifiers() & Qt::ControlModifier) )
            _cam->track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
        }
      }

      this->mousePosition_ = event->pos();
      triggerUpdate();
    }

    void SceneGLView::changeZoom(int _value)
    {
      if (!dataModel()) return;
      auto* _cam = dataModel()->scene().camera();
      if (!_cam) return;
      _cam->setDistance(_value/5.0);
      triggerUpdate();
    }

    void SceneGLView::dataToFrontend()
    {
      vizSession_.reset(new visual::Session(*dataModel()));
      initializeGL();
    }
  }
}

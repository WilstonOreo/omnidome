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

#include <omni/ui/GLView3D.h>

#include <QMouseEvent>
#include <omni/util.h>
#include <omni/visual/util.h>

namespace omni
{
  namespace ui
  {
    GLView3D::GLView3D(QWidget* _parent) :
      GLView(_parent)
    {
    }

    GLView3D::~GLView3D()
    {
    }

    bool GLView3D::initialize()
    {
      if (!dataModel() || initialized() || !context()) return false;

      dataModel()->scene().updateLights();
      grid_.reset(new visual::Grid(dataModel()->scene().camera()));
      grid_->update();

      this->vizSession_->update();
      return true;
    }

    void GLView3D::paintGL()
    {
      if (!dataModel() || this->isLocked() || !initialized()) return;

      auto& _scene = dataModel()->scene();

      this->vizSession_->update();
      glEnable(GL_DEPTH_TEST);

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      visual::viewport(this);

      _scene.camera().setup(30.0,aspect());

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      _scene.updateLights();

      this->vizSession_->drawCanvas(_scene.displayInput() && dataModel()->hasOutput() ?
          mapping::OutputMode::MAPPED_INPUT : mapping::OutputMode::LIGHTING_ONLY);

      this->vizSession_->drawProjectors(!_scene.displayProjectors());

      this->vizSession_->drawCanvasWithFrustumIntersections(_scene.projectorViewMode(),!_scene.displayProjectedAreas());
      this->vizSession_->drawProjectorHalos(!_scene.displayProjectors());

      if (_scene.displayGrid()) {
        grid_->draw(0.5);
        glDisable(GL_DEPTH_TEST);
        grid_->draw(0.5);
      }
    }

    void GLView3D::wheelEvent(QWheelEvent* event)
    {
      if (!dataModel()) return;

      auto& _cam = dataModel()->scene().camera();

      float _r = event->delta()/100.0;
      _cam.track( 0, 0, _r );

      if (dataModel()->canvas())
      {
        auto _r = dataModel()->canvas()->radius();
        _cam.limitDistance(_r*0.1,_r*10.0);
      }

      update();
    }

    void GLView3D::keyPressEvent(QKeyEvent* event)
    {
    }

    void GLView3D::mouseMoveEvent(QMouseEvent *event)
    {
      if (!dataModel()) return;

      auto& _cam = dataModel()->scene().camera();

      if (event->buttons() & Qt::LeftButton)
      {
        if( event->modifiers() & Qt::ShiftModifier )
        {
          _cam.strafe((event->pos().x() - mousePosition().x())/20.0);
          _cam.lift((event->pos().y() - mousePosition().y())/20.0);
        }
        else
        {
          if( !(event->modifiers() & Qt::ControlModifier) )
            _cam.track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
        }
        update();
      }

      this->mousePosition_ = event->pos();
    }

    void GLView3D::changeZoom(int _value)
    {
      if (!dataModel()) return;
      auto& _cam = dataModel()->scene().camera();
      _cam.setDistance(_value/5.0);
      update();
    }
  }
}

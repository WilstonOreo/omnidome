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
      GLView(_parent),
      grid_(camera_)
    {
    }

    GLView3D::~GLView3D()
    {
    }

    bool GLView3D::initialize()
    {
      if (!dataModel() || initialized() || !context()) return false;

      using namespace visual;
      float _radius = dataModel()->canvas() ? dataModel()->canvas()->radius() : 5.0;

      camera_ = Camera(
                  Tracker(QVector3D(0,0,0), PolarVec(-45.0,45.0,_radius * 5.0)));

      lights_[0] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(-45.0,45.0,_radius * 10.0)),1.0);

      lights_[1] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(45.0,45.0,_radius * 10.0)),0.2);

      lights_[2] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(45.0,-45.0,_radius * 10.0)),0.2);

      updateLight();
      grid_.update();
      this->vizSession_->update();
      return true;
    }

    void GLView3D::setupCamera()
    {
      camera_.setup(30.0,aspect());
    }

    void GLView3D::updateLight()
    {
      GLuint _index = GL_LIGHT0;
      for (auto& _light : lights_)
        _light.setup(++_index);
    }

    void GLView3D::paintGL()
    {
      if (!dataModel() || this->isLocked()) return;

      this->vizSession_->update();
      glEnable(GL_DEPTH_TEST);

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      visual::viewport(this);

      setupCamera();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      updateLight();

      this->vizSession_->drawCanvas(displayInput_ && dataModel()->hasOutput() ?
          mapping::OutputMode::MAPPED_INPUT : mapping::OutputMode::LIGHTING_ONLY);

      this->vizSession_->drawProjectors(!displayProjectors());

      this->vizSession_->drawCanvasWithFrustumIntersections(projectorViewMode_,!displayProjectedAreas());
      this->vizSession_->drawProjectorHalos(!displayProjectors());

      if (displayGrid()) {
        grid_.draw(0.5);
        glDisable(GL_DEPTH_TEST);
        grid_.draw(0.5);
      }
    }

    void GLView3D::wheelEvent(QWheelEvent* event)
    {
      if (!dataModel()) return;

      float _r = event->delta()/100.0;
      camera_.track( 0, 0, _r );

      if (dataModel()->canvas())
      {
        auto _r = dataModel()->canvas()->radius();
        camera_.limitDistance(_r*0.1,_r*10.0);
      }

      update();
    }

    void GLView3D::keyPressEvent(QKeyEvent* event)
    {
    }

    void GLView3D::mouseMoveEvent(QMouseEvent *event)
    {
      if (event->buttons() & Qt::LeftButton)
      {
        if( event->modifiers() & Qt::ShiftModifier )
        {
          camera_.strafe((event->pos().x() - mousePosition().x())/20.0);
          camera_.lift((event->pos().y() - mousePosition().y())/20.0);
        }
        else
        {
          if( event->modifiers() & Qt::ControlModifier )
          {
            for (auto& _light : lights_)
            {
              _light.track( event->pos().x() - mousePosition().x(), - event->pos().y() + mousePosition().y(), 0 );
            }
            updateLight();
          }
          if( !(event->modifiers() & Qt::ControlModifier) )
            camera_.track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
        }
        update();
      }

      this->mousePosition_ = event->pos();
    }

    bool GLView3D::displayInput() const
    {
      return displayInput_;
    }

    bool GLView3D::displayMeasures() const
    {
      return displayMeasures_;
    }

    bool GLView3D::displayGrid() const
    {
      return displayGrid_;
    }

    bool GLView3D::displayProjectors() const
    {
      return displayProjectors_;
    }

    bool GLView3D::displayProjectedAreas() const
    {
      return displayProjectedAreas_;
    }

    EditMode GLView3D::editMode() const {
        return editMode_;
    }

    RotateMode GLView3D::rotateMode() const {
        return rotateMode_;
    }

    MoveMode GLView3D::moveMode() const {
        return moveMode_;
    }

    void GLView3D::setDisplayInput(bool _displayInput)
    {
      displayInput_ = _displayInput;
      update();
    }

    void GLView3D::setDisplayMeasures(bool _displayMeasures) {
        displayMeasures_ = _displayMeasures;
        update();
    }

    void GLView3D::setDisplayGrid(bool _displayGrid) {
        displayGrid_ = _displayGrid;
        update();
    }

    void GLView3D::setDisplayProjectors(bool _displayProjectors) {
        displayProjectors_ = _displayProjectors;
        update();
    }

    void GLView3D::setDisplayProjectedAreas(bool _displayProjectedAreas) {
        displayProjectedAreas_ = _displayProjectedAreas;
        update();
    }

    void GLView3D::setEditMode(EditMode _editMode) {
        editMode_ = _editMode;
        update();
    }

    void GLView3D::setRotateMode(RotateMode _rotateMode) {
        rotateMode_ = _rotateMode;
        update();
    }

    void GLView3D::setMoveMode(MoveMode _moveMode) {
        moveMode_ = _moveMode;
        update();
    }

    ProjectorViewMode GLView3D::projectorViewMode() const
    {
      return projectorViewMode_;
    }

    void GLView3D::setProjectorViewMode(ProjectorViewMode _projectorViewMode)
    {
      projectorViewMode_ = _projectorViewMode;
      update();
    }

    void GLView3D::setProjectorViewMode(int _proj) {
       setProjectorViewMode(util::intToEnum<ProjectorViewMode>(_proj));
    }

    void GLView3D::changeZoom(int _value)
    {
      camera_.setDistance(_value/5.0);
      update();
    }
  }
}

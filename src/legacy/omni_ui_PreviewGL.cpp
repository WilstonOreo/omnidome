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

#include <omni/ui/PreviewGL.h>
#include <omni/Session.h>
#include <QMouseEvent>

#include <omni/proj/Frustum.hpp>
#include <omni/proj/Setup.hpp>
#include <omni/canvas/Interface.hpp>

using namespace omni::ui;

PreviewGL::PreviewGL(QWidget* _parent) :
  GLView(_parent),
  canvas_(nullptr),
  projSetup_(nullptr)
{
}

PreviewGL::~PreviewGL() {}

void PreviewGL::setProjSetup(proj::Setup const* _projSetup)
{
  projSetup_=_projSetup;
  update();
}

void PreviewGL::setCanvas(canvas::Interface const* _canvas)
{
  canvas_=_canvas;
  update();
}

void PreviewGL::updateLight()
{
  gex::Vec4 _pos(light_.eye().x(),light_.eye().y(),light_.eye().z(),0.0);

  // light and material
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_.diffuse());
  glLightfv(GL_LIGHT1, GL_POSITION, _pos.data());
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_.ambient());
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHT1);
}

void PreviewGL::initializeGL()
{
  float _radius = 10;
  // setup camera
  camera_ = Camera(
              Tracker(
                // target to track (origin)
                Point3(0,0,0),
                // set tracking position from spheric coordinates
                PolarVec(-45.0, 45.0, _radius * 1.5)
              ),
              // near, far
              1.0, 1000.0,
              Vec3(0.0, 0.0, 1.0)
            );
  // setup light source
  light_ = Light(
             Tracker(
               // target to track (origin)
               Point3(0.0,0.0,0.0),
               // set tracking position from spheric coordinates
               PolarVec(-45.0, 45.0, _radius * 10)
             ),
             // ambient color
             Color4f(0.2, 0.2, 0.2, 1.0),
             // diffuse color
             Color4f(0.8, 0.8, 0.8, 1.0),
             // specular color,
             Color4f(0.6, 0.6, 0.6, 1.0),
             // intensity
             1.0,
             // shadows
             0.0,
             // radius
             1.0
           );
  updateLight();

  GLView::initializeGL();
}

void PreviewGL::resizeGL(int _w, int _h)
{
  GLView::resizeGL(_w,_h);
  // reshaped window aspect ratio
  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // perspective projection
  gluPerspective(30.0, aspect(), 1.0/*camera_.near()*/, camera_.far());
  glMatrixMode(GL_MODELVIEW);
}

void PreviewGL::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!canvas_ || !projSetup_) return;
  if (!initialized()) initializeGL();

  auto&& _projectors = projSetup_->setup(canvas_);

  auto _mappingColors = Session::mappingColors();
  _mappingColors.resize(_projectors.size());

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, aspect(),
                 1.0, camera().far());

  // realize coordinates
  gluLookAt(
    camera_.eye().x(),
    camera_.eye().y(),
    camera_.eye().z(),
    camera_.center().x(),
    camera_.center().y(),
    camera_.center().z(),
    camera_.up().x(),
    camera_.up().y(),
    camera_.up().z()
  );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  auto&& _bounds  = canvas_->bounds();
  auto _length = _bounds.size().norm();

  size_t i = 0;
  for (auto& _proj : _projectors)
  {
    drawProjector(
      _proj,
      proj::Frustum(_proj),
      _mappingColors[i],
      _length/_projectors.size()/2.0);
    ++i;
  }

  glDisable(GL_LIGHTING);

  canvas_->drawAux();
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  drawProjectorSectors(canvas_->radius(),_mappingColors);
  glDisable(GL_CULL_FACE);

  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);
  glColor4f(1.0,1.0,1.0,0.5);
  canvas_->drawCanvas();

  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  glDepthMask (GL_FALSE);

  i = 0;
  for (auto& _proj : _projectors)
  {
    drawProjectorHalo(proj::Frustum(_proj),_mappingColors[i],_length);
    ++i;
  }
  glDepthMask(GL_TRUE);
  glEnable(GL_LIGHTING);

  swapBuffers();
}

void PreviewGL::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton)
  {
    if( event->modifiers() & Qt::ShiftModifier )
    {
      camera_.strafe((event->pos().x() - mousePosition().x())/2.0);
      camera_.lift((event->pos().y() - mousePosition().y())/2.0);
    }
    else
    {
      if( event->modifiers() & Qt::ControlModifier )
      {
        light_.track( event->pos().x() - mousePosition().x(), - event->pos().y() + mousePosition().y(), 0 );
        updateLight();
      }
      if( !(event->modifiers() & Qt::ControlModifier) )
        camera_.track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
    }
    update();
  }
  mousePosition() = event->pos();
}

void PreviewGL::mousePressEvent(QMouseEvent *event)
{
  mousePosition() = event->pos();
  switch (event->button())
  {
  case Qt::LeftButton:
    update();
    break;
  default:
    ;
  }
}
void PreviewGL::wheelEvent(QWheelEvent* event)
{
  camera_.track( 0, 0, (double)event->delta()/100.0 );
  update();
}

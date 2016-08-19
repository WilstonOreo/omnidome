#include "OpenGLCoreWidget.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <omni/visual/util.h>
#include <omni/proj/PeripheralSetup.h>


namespace omni {
  OpenGLCoreWidget::OpenGLCoreWidget(QWidget* _parent) :
    QOpenGLWidget(_parent),
    mousePos_(0.0,0.0) {

    proj::SetupFactory::reg<proj::PeripheralSetup>();
  }

  OpenGLCoreWidget::~OpenGLCoreWidget() {

  }

  float OpenGLCoreWidget::aspect() const
  {
    return (float)width() / height();
  }


  void OpenGLCoreWidget::keyPressEvent(QKeyEvent *event)
  {

  }

  void OpenGLCoreWidget::mousePressEvent(QMouseEvent* event) {
    QOpenGLWidget::mousePressEvent(event);
    setFocus();
    mousePos_ = event->pos();
    update();
  }

  void OpenGLCoreWidget::mouseMoveEvent(QMouseEvent* event) {
    QOpenGLWidget::mouseMoveEvent(event);

    if (event->buttons() & Qt::LeftButton)
    {
      QPointF d = event->pos() - mousePos_;
      if (event->modifiers() & Qt::ShiftModifier) {
        cam_.strafe(d.x() / 20.0);
        cam_.lift(d.y() / 20.0);
      } else {
        if (!(event->modifiers() & Qt::ControlModifier)) cam_.track(
          d.x(),d.y(), 0);
      }
      update();
    }
  }

  void OpenGLCoreWidget::wheelEvent(QWheelEvent* event) {
    float _r = event->delta() / 100.0;
    cam_.track(0, 0, _r);

    auto _size = 20.0f;
    cam_.limitDistance(_size * 0.1, _size * 5.0);

    update();
  }

  void OpenGLCoreWidget::initializeGL() {
    initializeOpenGLFunctions();
  }

  void OpenGLCoreWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    visual::viewport(this);

    cam_.setup(aspect());

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    projector_.setup("PeripheralSetup");
    vizProjector_.reset(new visual::Projector(projector_));

    vizProjector_->update();
    vizProjector_->draw();
  }
}

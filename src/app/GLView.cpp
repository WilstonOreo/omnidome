#include <omni/ui/GLView.h>

#include <omni/Session.h>
#include <omni/util.h>
#include <omni/visual/util.h>

#include <QMouseEvent>


namespace omni
{
  namespace ui
  {
    GLView::GLView(QWidget* _parent) :
      QOpenGLWidget(_parent)
    {
      QSurfaceFormat _format(QSurfaceFormat::ResetNotification); 
      setFormat(_format);
    }
    
    GLView::~GLView()
    {
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

    void GLView::hideEvent(QHideEvent * event)
    {
      qDebug() << "hidden";
      doneCurrent();
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
      });


      initialized_ = initialize();
    }

    void GLView::resizeGL(int _w, int _h)
    {
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
      if (!session_) return;
    }

    Session const* GLView::session() const
    {
      return !session_ ? nullptr : &session_->session();
    }

    void GLView::setSession(Session* _session)
    {
      if (!_session) return;

      session_.reset(new visual::Session(*_session));
      initialized_ = initialize();
    }
 
    void GLView::mousePressEvent(QMouseEvent* event)
    {
      QOpenGLWidget::mousePressEvent(event);
      setFocus();
      if (!session_) return;
      mousePosition_ = event->pos();

      update();
    }
  }
}

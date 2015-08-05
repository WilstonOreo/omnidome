#include <omni/ui/GLView3D.h>

#include <QMouseEvent>
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
      if (!session() || initialized()) return false;

      using namespace visual;
      float _radius = 5.0;

      camera_ = Camera(
                  Tracker(QVector3D(0,0,0), PolarVec(-45.0,45.0,_radius * 10.0)));

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
      this->session_->update();
      return true;
    }
    
    void GLView3D::updateLight()
    {
      GLuint _index = GL_LIGHT0;
      for (auto& _light : lights_)
        _light.setup(++_index);
    }

    void GLView3D::paintGL()
    {
      if (!session()) return;

      visual::with_current_context([this](QOpenGLFunctions& _)
      {
        _.glViewport(0, 0, (GLint)width(), (GLint)height());
      });
      this->session_->update();

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      visual::viewport(this);
      
      camera_.setup(30.0,aspect());
     
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      
  
      this->session_->drawProjectors();

      updateLight();
      this->session_->drawCanvas();
      this->session_->drawCanvasWithFrustumIntersections();
      this->session_->drawProjectorHalos();
    }

    void GLView3D::wheelEvent(QWheelEvent* event)
    {
      if (!session()) return;
      
      camera_.track( 0, 0, (double)event->delta()/100.0 );
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
          camera_.strafe((event->pos().x() - mousePosition().x())/2.0);
          camera_.lift((event->pos().y() - mousePosition().y())/2.0);
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

    void GLView3D::changeZoom(int _value)
    {
      camera_.setDistance(_value/5.0);
      update();
    }
  }
}


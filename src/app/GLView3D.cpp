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
      if (!session() || initialized() || !context()) return false;

      using namespace visual;
      float _radius = session()->canvas() ? session()->canvas()->radius() : 5.0;

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
      this->session_->update();
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
      if (!session()) return;

      this->session_->update();

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      visual::viewport(this);

      setupCamera();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      updateLight();
      this->session_->drawCanvas(Mapping::MAPPED_INPUT,displayInput_);
      this->session_->drawProjectors();
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
      
    bool GLView3D::displayInput() const
    {
      return displayInput_;
    }

    void GLView3D::setDisplayInput(bool _displayInput)
    {
      displayInput_ = _displayInput;
      update();
    }

    void GLView3D::changeZoom(int _value)
    {
      camera_.setDistance(_value/5.0);
      update();
    }
  }
}


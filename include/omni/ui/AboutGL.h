#ifndef OMNI_UI_ABOUTGL_H_
#define OMNI_UI_ABOUTGL_H_

#include <memory>
#include <omni/gl.hpp>
#include <QtOpenGL/QGLWidget>
#include <tbd/property.h>

namespace omni
{  
  namespace ui
  {
    class AboutGL : public QGLWidget
    {
      Q_OBJECT
    public:
      explicit AboutGL(QWidget* _parent = nullptr);
      ~AboutGL();
      
      TBD_PROPERTY_REF_RO(std::unique_ptr<Shader>,shader)
    protected:
      void timerEvent ( QTimerEvent * event );

      void initializeGL();
      void resizeGL(int _w, int _h);    
      void paintGL();
      double startTime_;
    };
  }
}

#endif /* OMNI_UI_ABOUTGL_H_ */

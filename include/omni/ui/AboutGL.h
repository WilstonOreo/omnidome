#ifndef OMNI_UI_ABOUTGL_H_
#define OMNI_UI_ABOUTGL_H_

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

namespace omni
{  
  namespace ui
  {
    class AboutGL : public QOpenGLWidget
    {
      Q_OBJECT
    public:
      explicit AboutGL(QWidget* _parent = nullptr);
      ~AboutGL();
      
    protected:
      void mousePressEvent(QMouseEvent*);

      void initializeGL();
      void resizeGL(int _w, int _h);    
      void paintGL();
      double startTime_;

    private:
      QOpenGLShaderProgram* shader_ = nullptr;
    };
  }
}

#endif /* OMNI_UI_ABOUTGL_H_ */

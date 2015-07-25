#ifndef OMNI_UI_GLVIEW_H_
#define OMNI_UI_GLVIEW_H_

#include <set>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPointF>
#include <omni/visual/Session.h>

namespace omni
{
  class Session;

  namespace ui
  {
    class GLView : 
      public QOpenGLWidget,
      protected QOpenGLFunctions
    {
      Q_OBJECT
    public:
      explicit GLView(QWidget* _parent = nullptr);
      GLView(omni::Session*,QWidget* _parent = nullptr);
      ~GLView();

      typedef std::set<GLView*> view_set_type;

      float aspect() const;
      Session const* session() const;
      void setSession(Session* _session);

      QPointF mousePosition() const;

      bool initialized() const;

    protected:
      void initializeGL();
      virtual void resizeGL(int _w, int _h);
      virtual void paintGL();

      virtual void mousePressEvent(QMouseEvent*);
      
      QPointF mousePosition_;
      std::unique_ptr<visual::Session> session_;

    private:
      virtual bool initialize() = 0;
      bool initialized_ = false;
    };
  }
}

#endif /* OMNI_UI_GLVIEW_H_ */

#ifndef OMNI_UI_GLVIEW_H_
#define OMNI_UI_GLVIEW_H_

#include <set>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPointF>

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
      ~GLView();

      typedef std::set<GLView*> view_set_type;

      /// Frees textures
      virtual void free();

      float aspect() const;

      Session const* session() const;
      void setSession(Session* _session);

      QPointF mousePosition() const;

    protected:
      virtual void initializeGL();
      virtual void resizeGL(int _w, int _h);
      virtual void paintGL();

      virtual void mousePressEvent(QMouseEvent*);
      
      QPointF mousePosition_;

    private:

      Session* session_;      
    
    };
  }
}

#endif /* OMNI_UI_GLVIEW_H_ */

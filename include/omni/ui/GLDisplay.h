#ifndef OMNI_UI_GLDISPLAY_H_
#define OMNI_UI_GLDISPLAY_H_ 

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

namespace omni
{  
  namespace ui
  {
    class GLDisplay : 
      public QOpenGLWidget,
      protected QOpenGLFunctions
    {
      Q_OBJECT
    public:
      explicit GLDisplay(QWidget* _parent = nullptr);
      ~GLDisplay();
 
    protected:
      void initializeGL();
      void resizeGL(int _w, int _h);    
      void paintGL();

    private:
      GLuint texId_;

//      QOpenGLTexture* tex_;
    };
  }
}

#endif /* OMNI_UI_GLDISPLAY_H_ */

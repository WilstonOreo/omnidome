#ifndef OMNI_UI_GLVIEW2D_H_
#define OMNI_UI_GLVIEW2D_H_

#include <omni/ui/GLView.h>

namespace omni
{
  namespace ui
  {
    class GLView2D : public GLView
    {
      Q_OBJECT
    public:
      GLView2D(QWidget* = nullptr);
      ~GLView2D();
 
    public slots:
      void setKeepAspectRatio(bool);

    protected:
      void mouseMoveEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent* event);
      void keyPressEvent(QKeyEvent* event);

    private:

      bool keepAspectRatio_ = false;
    };
  }
}


#endif /* OMNI_UI_GLVIEW2D_H_ */

#ifndef OMNI_UI_CANVASGL_H_
#define OMNI_UI_CANVASGL_H_ 

#include <omni/core.hpp>
#include <omni/visual.hpp>
#include "GLView.h"
 
namespace omni
{
  class Session;
  
  namespace ui
  {
    class CanvasGL : public GLView
    {
      Q_OBJECT
    public:
      explicit CanvasGL(QWidget *parent = nullptr);
      ~CanvasGL();

      void initialize(Session* _session, const view_set_type& _views);
    
      TBD_PROPERTY_REF(Camera,camera)
      TBD_PROPERTY_REF(Light,light)

      TBD_PROPERTY_REF(bool,inside)

    public slots:
      void changeZoom(int);

    protected:
      void mouseMoveEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void keyPressEvent(QKeyEvent* even);
      void initializeGL();
      void resizeGL(int _w, int _h);
      void paintGL();
   private:
      void updateLight();

      void drawPositioning();
      float oldZoom_;
    };
  }
}

#endif /* OMNI_UI_CANVASGL_H_ */

#ifndef OMNI_UI_PREVIEWGL_H_
#define OMNI_UI_PREVIEWGL_H_

#include <omni/core.hpp>
#include <omni/visual.hpp>
#include "GLView.h"

namespace omni
{
  namespace proj
  {
    class Setup;
  }

  namespace canvas
  {
    class Interface;
  }

  namespace ui
  {
    class PreviewGL : public GLView
    {
      Q_OBJECT
    public:
      explicit PreviewGL(QWidget *parent = nullptr);
      ~PreviewGL();
      
      void setProjSetup(proj::Setup const* _projSetup);
      void setCanvas(canvas::Interface const* _canvas);

      TBD_PROPERTY_REF(Camera,camera)
      TBD_PROPERTY_REF(Light,light)
   
    protected:
      void initializeGL();
      void resizeGL(int _w, int _h);
      void paintGL();
      void mouseMoveEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent *event);
      void mousePressEvent(QMouseEvent *event);
    private:
      void updateLight();

      canvas::Interface const* canvas_;
      proj::Setup const* projSetup_;
    };
  }
}

#endif /* OMNI_UI_PREVIEWGL_H_ */

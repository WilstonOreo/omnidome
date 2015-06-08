#include <omni/ui/GLView2D.h>

#include <QDebug>
#include <QResizeEvent>

namespace omni
{
  namespace ui
  {
    GLView2D::GLView2D(QWidget* _parent) :
      GLView(_parent)
    {
//      QSizePolicy qsp(QSizePolicy::Fixed,QSizePolicy::Expanding);
//      this->setSizePolicy(qsp);
    }

    GLView2D::~GLView2D()
    {
    }

    void GLView2D::setKeepAspectRatio(bool _keepAspectRatio)
    {
      sizePolicy().setHeightForWidth(_keepAspectRatio);
      update();
    }

    void GLView2D::mouseMoveEvent(QMouseEvent *event)
    {
    }

    void GLView2D::mousePressEvent(QMouseEvent *event)
    {
      GLView::mousePressEvent(event);
    }

    void GLView2D::mouseReleaseEvent(QMouseEvent *event)
    {
    }

    void GLView2D::wheelEvent(QWheelEvent* event)
    {
    }

    void GLView2D::keyPressEvent(QKeyEvent* event)
    {
    }

  }
}

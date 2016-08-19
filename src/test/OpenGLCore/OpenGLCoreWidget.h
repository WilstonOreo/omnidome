#ifndef OMNI_OPENGLCOREWIDGET_H_
#define OMNI_OPENGLCOREWIDGET_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QPointF>
#include <QOpenGLShaderProgram>
#include <omni/visual/PerspectiveCamera.h>
#include <omni/visual/Projector.h>

namespace omni {
  class OpenGLCoreWidget :
    public QOpenGLWidget,
    protected QOpenGLFunctions {
    Q_OBJECT
  public:
    OpenGLCoreWidget(QWidget* = nullptr);
    ~OpenGLCoreWidget();

    float aspect() const;

  protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);

  private:
    void initializeGL();
    void paintGL();

    QPointF mousePos_;
    visual::PerspectiveCamera cam_;

    proj::Projector projector_;
    std::unique_ptr<visual::Projector> vizProjector_;
  };
}

#endif /* OMNI_OPENGLCOREWIDGET_H_ */

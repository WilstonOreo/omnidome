#include <omni/ui/CanvasGL.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <omni/Session.h>
#include <omni/Mapping.h>
#include <omni/proj.hpp>
#include <omni/canvas/util.hpp>
#include <QMouseEvent>

using namespace omni::ui;

omni::Point3 unProject(QPoint const & pos)
{
  GLdouble projection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  GLdouble modelView[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  double winX = pos.x();
  double winY = viewport[3] - pos.y();
  GLfloat winZ;
  glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
  GLdouble x,y,z;
  gluUnProject(winX, winY, winZ, modelView, projection, viewport, &x, &y, &z);
  return omni::Point3(x,y,z);
}

CanvasGL::CanvasGL(QWidget *parent) :
  GLView(parent),
  inside_(false),
  oldZoom_(0.0)
{
}

CanvasGL::~CanvasGL()
{
}

void CanvasGL::initializeGL()
{
  float _radius = 10;
  // setup camera
  camera_ = Camera(
              Tracker(
                // target to track (origin)
                Point3(0,0,0),
                // set tracking position from spheric coordinates
                PolarVec(-45.0, 45.0, _radius * 1.5)
              ),
              // near, far
              1.0, 1000.0,
              Vec3(0.0, 0.0, 1.0)
            );
  // setup light source
  light_ = Light(
             Tracker(
               // target to track (origin)
               Point3(0.0,0.0,0.0),
               // set tracking position from spheric coordinates
               PolarVec(-45.0, 45.0, _radius * 10)
             ),
             // ambient color
             Color4f(0.2, 0.2, 0.2, 1.0),
             // diffuse color
             Color4f(0.8, 0.8, 0.8, 1.0),
             // specular color,
             Color4f(0.6, 0.6, 0.6, 1.0),
             // intensity
             1.0,
             // shadows
             0.0,
             // radius
             1.0
           );
  updateLight();

  GLView::initializeGL();
}

void CanvasGL::resizeGL(int _w, int _h)
{
  GLView::resizeGL(_w,_h);
  // reshaped window aspect ratio
  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // perspective projection
  gluPerspective(30.0, aspect(), 1.0/*camera_.near()*/, camera_.far());
  glMatrixMode(GL_MODELVIEW);
}


void CanvasGL::drawPositioning()
{
  /*
   QGLShaderProgram program(context());
   program.addShaderFromSourceCode(QGLShader::Vertex,
   "attribute highp vec4 vertex;\n"
  "uniform highp mat4 matrix;\n"
  "void main(void)\n"
  "{\n"
  "   gl_Position = matrix * vertex;\n"
  "}");
  program.addShaderFromSourceCode(QGLShader::Fragment,
  "uniform mediump vec4 color;\n"
  "void main(void)\n"
  "{\n"
  "   gl_FragColor = color;\n"
  "}");
  program.link();
  program.bind();


  int vertexLocation = program.attributeLocation("vertex");
  int matrixLocation = program.uniformLocation("matrix");
  int colorLocation = program.uniformLocation("color");*/
}

void CanvasGL::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!session()) return;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, aspect(),
                 1.0, camera().far());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  auto _canvas = session()->canvas().get();
  if (!_canvas) return;

  auto&& _bounds  = _canvas->bounds();
  auto _length = _bounds.size().norm();
  auto&& _mappings = session()->fetchMappings();

  std::vector<std::pair<Mapping const*,Frustum const>> _frusta;
  for (auto& _mapping : _mappings)
    if (!excludedProjectors().count(&_mapping->projector()))
      _frusta.emplace_back(_mapping,proj::Frustum(_mapping->projector(),_mapping->aspectRatio()));

  glDisable(GL_LIGHTING);
  for (auto& _frustum : _frusta)
  {
    drawProjector(
      _frustum.first->projector(),
      _frustum.second,
      _frustum.first->color(),
      _length/_mappings.size()/2.0);
  }

  bool _drawAux = !_frusta.empty();
  if (_drawAux)
  {
    session()->canvas()->drawAux();

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    //      drawProjectorSectors(session()->numMappings());
    glDisable(GL_CULL_FACE);
  }


  glDisable(GL_CULL_FACE);
  glLineWidth(1.0);
  auto _mode = session()->mode();
  switch (_mode)
  {
  case Session::CANVAS_PROJECTORS:
  case Session::POSITIONING:
    if (inside())
    {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }
    drawCanvas(_frusta.empty() ? 1.0 : 0.25);
    for (auto& _frustum : _frusta)
    {
      drawCanvas(_frustum.second,_frustum.first->color());
    }
    break;
  case Session::SIMULATE:
  case Session::EXPORT:
    if (inside())
    {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }
    drawCanvas(drawBorders() ? 0.25 : 1.0);
    if (drawBorders())
    {
      for (auto& _mapping : _mappings)
      {
        drawCanvas(
          proj::Frustum(_mapping->projector(),_mapping->aspectRatio()),
          _mapping->color());
      }
    }
    break;
  case Session::WARP:
  case Session::BLEND:
    break;
  }

  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  glDepthMask (GL_FALSE);
  for (auto& _frustum : _frusta)
  {
    drawProjectorHalo(_frustum.second,_frustum.first->color(),_length);
  }
  glDepthMask(GL_TRUE);
  glEnable(GL_LIGHTING);

  swapBuffers();
}

void CanvasGL::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton)
  {
    if( event->modifiers() & Qt::ShiftModifier )
    {
      camera_.strafe((event->pos().x() - mousePosition().x())/2.0);
      camera_.lift((event->pos().y() - mousePosition().y())/2.0);
    }
    else
    {
      if( event->modifiers() & Qt::ControlModifier )
      {
        light_.track( event->pos().x() - mousePosition().x(), - event->pos().y() + mousePosition().y(), 0 );
        updateLight();
      }
      if( !(event->modifiers() & Qt::ControlModifier) )
        camera_.track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
    }
    update();
  }
  mousePosition() = event->pos();
}

void CanvasGL::mousePressEvent(QMouseEvent *event)
{
  if (!session()) return;
  mousePosition() = event->pos();
  switch (event->button())
  {
  case Qt::LeftButton:
    //selection_ = unProject(event->pos()); ///@todo selection by click
    update();
    break;
  default:
    ;
  }
}
void CanvasGL::wheelEvent(QWheelEvent* event)
{
  if (!session()) return;

  oldZoom_ = event->delta() / 100.0;
  camera_.track( 0, 0, (double)event->delta()/100.0 );
  update();
}
void CanvasGL::keyPressEvent(QKeyEvent* event)
{
  if (!session()) return;
  int step = 1;
  if( event->modifiers() & Qt::ShiftModifier )
    step *= 10;
  int x=0, y=0, z=0;
  switch( event->key() )
  {
  case Qt::Key_PageUp:
    z = -step;
    break;
  case Qt::Key_Up:
    y = -step;
    break;
  case Qt::Key_Left:
    x = -step;
    break;
  case Qt::Key_PageDown:
    z = step;
    break;
  case Qt::Key_Down:
    y = step;
    break;
  case Qt::Key_Right:
    x = step;
    break;
  }

  update();
  QGLWidget::keyPressEvent(event);
}

void CanvasGL::changeZoom(int _value)
{
  camera_.distance(_value/5.0);
  update();
}


#include <omni/ui/AboutGL.h>

#include <QFile>
#include <QTimer>
#include <chrono>
#include <type_traits>
#include <omni/gl.hpp>

using namespace omni::ui;

static std::string qfileToStr(const std::string& _filename)
{
  QFile _f(QString().fromStdString(_filename));
  _f.open(QIODevice::ReadOnly|QIODevice::Text);
  return _f.readAll().constData();
}



AboutGL::AboutGL(QWidget* _parent) :
  QGLWidget(
    QGLFormat(
      QGL::DoubleBuffer |
      QGL::DepthBuffer |
      QGL::Rgba |
      QGL::AlphaChannel |
      QGL::DirectRendering
    ),_parent)
{
  QTimer *timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()), this, SLOT(updateGL()));
  timer->start(20);
  startTime_ =
    std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000000.0 +
    double(reinterpret_cast<long long>(timer) & ((1 << 16) - 1) );
}

AboutGL::~AboutGL()
{
}


void AboutGL::initializeGL()
{
// No glew required for apple
#ifndef __APPLE__
  //glewExperimental=GL_TRUE; // Enable experimental extension for OpenGL 3.3 and later
  static GLenum err = -1;
  if (err == -1)
  {
    err = glewInit();
    if (GLEW_OK != err)
    {
      std::cerr << "GLEW could not be initialized. Error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
      std::cerr << "GLEW was initialized!" << std::endl;
    }
  }
#endif

  enable(GL_DEPTH_TEST,GL_BLEND,GL_POINT_SMOOTH,GL_LINE_SMOOTH,GL_NORMALIZE);

  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  static std::string _vertSrc = qfileToStr(":/shaders/frustum.vert");
  static std::string _fragmentSrc = qfileToStr(":/shaders/slow_fractal.frag");

  if (!shader_)
    shader_.reset(new Shader(_vertSrc,_fragmentSrc));

  setAutoBufferSwap(true);
}

void AboutGL::resizeGL(int _w, int _h)
{
  _w = _w & ~1;
  _h = _h & ~1;
  glViewport(0, 0, (GLint)_w, (GLint)_h);
  glClearColor(0.0,0.0,0.0,1.0);
}

void AboutGL::timerEvent ( QTimerEvent * event )
{
}

void AboutGL::paintGL()
{
  if (!shader()) return;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable( GL_CULL_FACE );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-0.5,0.5,-0.5,0.5);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  double _time = startTime_ -
                 std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000000.0 ;

  use_shader(*shader(),[&]()
  {
    shader()->uniform("time",GLfloat(_time));
    shader()->uniform("resolution",GLfloat(width()),GLfloat(height()));
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_FILL);

    draw(GL_QUADS,[]()
    {
      texCoord(0,0);
      vertex(-0.5f,-0.5f);
      texCoord(1,0);
      vertex(0.5f,-0.5f);
      texCoord(1,1);
      vertex(0.5f,0.5f);
      texCoord(0,1);
      vertex(-0.5f,0.5f);
    });
  });

  renderText(10,30,"(C) 2014. Written by Wilston Oreo.");
  renderText(10,60,"GUI design by Brook Cronin + Michael Winkelmann");
  renderText(10,90,"Version 0.4");
  renderText(10,120,"http://omnido.me");
  renderText(10,150,"This is a preliminary version.");
}

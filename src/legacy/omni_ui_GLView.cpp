#include <omni/ui/GLView.h>

#include <omni/Session.h>
#include <omni/canvas/util.hpp>
#include <omni/gl.hpp>

#include <QFile>
#include <QByteArray>

using namespace omni::ui;

GLView::GLView(QWidget* _parent) :
  QGLWidget(
    QGLFormat(
      QGL::DoubleBuffer |
      QGL::DepthBuffer |
      QGL::Rgba |
      QGL::AlphaChannel |
      QGL::DirectRendering
    ),_parent),
  session_(nullptr),
  testTexture_(nullptr),
  mousePosition_(0,0),
  cursorPosition_(0,0),
  initialized_(false),
  drawBorders_(false),
  rotateX_(0),
  rotateY_(0),
  rotateZ_(0),
  whiteBlendMask_(false),
  mapMode_(MapMode::NONE)
{
  makeCurrent();
// No glew required for apple
#ifndef __APPLE__
  glewExperimental=GL_TRUE; // Enable experimental extension for OpenGL 3.3 and later
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
}

GLView::~GLView()
{
}

static std::string qfileToStr(const std::string& _filename)
{
  QFile _f(QString().fromStdString(_filename));
  _f.open(QIODevice::ReadOnly|QIODevice::Text);
  return _f.readAll().constData();
}

void GLView::initialize(Session* _session, const view_set_type& _views)
{
  makeCurrent();
  session_=_session;
  views_=_views;

  static std::string _vertSrc = qfileToStr(":/shaders/frustum.vert");
  static std::string _fragmentSrc = qfileToStr(":/shaders/frustum.frag");
  static std::string _blendVertSrc = qfileToStr(":/shaders/frustum.vert");
  static std::string _blendFragmentSrc = qfileToStr(":/shaders/blend.frag");

  if (!shader_)
    shader_.reset(new Shader(_vertSrc,_fragmentSrc));
  if (!blendShader_)
    blendShader_.reset(new Shader(_blendVertSrc,_blendFragmentSrc));
}

/// Frees textures
void GLView::free()
{
  makeCurrent();
  testTexture_.reset(nullptr);
  shader_.reset(nullptr);
  blendShader_.reset(nullptr);
}

void GLView::initializeGL()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);
  // fix outlines z-fighting with quads
  glPolygonOffset(1, 1);

  useTexture();

  setAutoBufferSwap(false);
  initialized_ = true;
}

void GLView::drawCanvas(float _alpha) const
{
  if (!session()) return;

  use_texture(*testTexture(),[&]()
  {
    use_shader(*shader(),[&]()
    {
      shader()->uniform("tex_alpha",GLfloat(_alpha));
      shader()->uniform("map_only",GLint(1));
      shaderUniforms();
      session()->canvas()->drawCanvas();
    });
  });
}

void GLView::shaderUniforms() const
{
  shader()->uniform("texture",*testTexture());
  shader()->uniform("map_mode",GLint(mapMode_));
  shader()->uniform("map_roll",GLfloat(rotateX()));
  shader()->uniform("map_pitch",GLfloat(rotateY()));
  shader()->uniform("map_yaw",GLfloat(rotateZ()));
}

void GLView::drawCanvas(
  const proj::Frustum& _f,
  const Color4f& _color) const
{
  if (!session()) return;

  use_texture(*testTexture(),[&]()
  {
    use_shader(*shader(),[&]()
    {
      shader()->uniform("eye",_f.eye()).
      uniform("look_at",_f.look_at()).
      uniform("top_left",_f.top_left()).
      uniform("top_right",_f.top_right()).
      uniform("bottom_left",_f.bottom_left()).
      uniform("bottom_right",_f.bottom_right()).
      uniform("color",_color);

      shader()->uniform("no_borders",GLint(1));
      shader()->uniform("tex_alpha",GLfloat(0.5f));
      shader()->uniform("map_only",GLint(0));
      shaderUniforms();
      session()->canvas()->drawCanvas();
    });
  });
}

void GLView::useTexture()
{
  if (!session()) return;
  makeCurrent();
  if (!testTexture_)
  {
    testTexture_.reset(new texture_type());
  }
  else
  {
    testTexture_->free();
  }
  testTexture_->initialize(session_->testImage().get());
  use_texture(*testTexture(),[&]()
  {
    texture_type::wrap(GL_REPEAT,GL_REPEAT);
    texture_type::min_mag(GL_LINEAR,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  });

  mapMode_ = MapMode::NONE;
}

void GLView::useTexture(std::string const& _filename, MapMode _mapMode)
{
  if (!session()) return;
  makeCurrent();
  if (session()->images().count(_filename) == 0) return;

  if (!testTexture_)
  {
    testTexture_.reset(new texture_type());
  }
  else
  {
    testTexture_->free();
  }
  mapMode_ = _mapMode;
  testTexture_->initialize(session_->getImage(_filename));
  use_texture(*testTexture(),[&]()
  {
    texture_type::wrap(GL_REPEAT,GL_REPEAT);
    texture_type::min_mag(GL_LINEAR,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  });
  update();
}

void GLView::resizeGL(int _w, int _h)
{
  _w = _w & ~1;
  _h = _h & ~1;
  glViewport(0, 0, (GLint)_w, (GLint)_h);
  glClearColor(0.0,0.0,0.0,1.0);
}

float GLView::aspect() const
{
  return (float)width() / height();
}

void GLView::drawLine(const Point3& _a, const Point3& _b)
{
  glVertex3f(_a.x(),_a.y(),_a.z());
  glVertex3f(_b.x(),_b.y(),_b.z());
}


void GLView::drawProjectorHalo(proj::Frustum const& _f, const Color4f& _color, float _size)
{
  Point3 _topLeft = _f.eye() +  _size*_f.top_left();
  Point3 _topRight = _f.eye() + _size*_f.top_right();
  Point3 _bottomLeft = _f.eye() + _size*_f.bottom_left();
  Point3 _bottomRight = _f.eye() + _size*_f.bottom_right();
  glBegin(GL_TRIANGLE_FAN);
  glColor4f(_color.r(),_color.g(),_color.b(),0.15);
  glVertex3f(_f.eye().x(),_f.eye().y(),_f.eye().z());
  glColor4f(_color.r(),_color.g(),_color.b(),0.00);
  drawLine(_topLeft,_topRight);
  drawLine(_bottomRight,_bottomLeft);
  glVertex3f(_topLeft.x(),_topLeft.y(),_topLeft.z());
  glEnd();
}

void GLView::drawProjectorSectors(float _radius, std::vector<Color4f> const& _colors)
{
  if (_colors.empty()) return;

  float _arcRads = 2.0 * M_PI / _colors.size();

  size_t i = 0;
  glDisable(GL_LIGHTING);
  for (auto& _color : _colors)
  {
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(_color.r(),_color.g(),_color.b(),0.25);
    glVertex3f(0,0,0);
    Point2 _first;
    canvas::util::for_each_arc_point(32,_radius,
                                     M_PI + (i - 0.5 )* _arcRads,M_PI + (i + 0.5 )* _arcRads,
                                     [&](size_t j, const Point2& _p)
    {
      if (j == 0) _first = _p;
      glVertex3f(_p.x(),_p.y(),0.0);
    });
    glEnd();
    ++i;
  }
  glEnable(GL_LIGHTING);
}

void GLView::drawProjector(
  proj::Projector const& _proj,
  proj::Frustum const& _f,
  const Color4f& _color,
  float _size)
{
  Point3 _topLeft = _f.eye() + _size*_f.top_left();
  Point3 _topRight = _f.eye() + _size*_f.top_right();
  Point3 _bottomLeft = _f.eye() + _size*_f.bottom_left();
  Point3 _bottomRight = _f.eye() + _size*_f.bottom_right();

  glColor4f(_color.r(),_color.g(),_color.b(),1.0);
  glLineWidth(1.0);
  glBegin(GL_LINES);
  drawLine(_f.eye(),_topLeft);
  drawLine(_f.eye(),_topRight);
  drawLine(_f.eye(),_bottomLeft);
  drawLine(_f.eye(),_bottomRight);
  drawLine(_topLeft,_topRight);
  drawLine(_topLeft,_bottomLeft);
  drawLine(_topRight,_bottomRight);
  drawLine(_bottomLeft,_bottomRight);
  glEnd();

  Point3 _origin(0,0,0);
  Scalar _theta = deg2rad(_proj.yaw());
  Scalar _ct = -cos(_theta), _st = -sin(_theta);
  Vec2 _shiftVec = Vec2(-_st,_ct) * _proj.shift();
  Vec2 _p = _proj.distance_center() * Vec2(_ct,_st);
  Point3 _yawPos(_p.x(),_p.y(),0.0);
  _p += _shiftVec;
  Point3 _shiftPos(_p.x(),_p.y(),0.0);

  glLineWidth(3.0);
  glBegin(GL_LINES);
  glColor4f(_color.r(),_color.g(),_color.b(),0.5);
  drawLine(_f.eye(),_f.eye() + _f.look_at()*_size*0.5);

  glColor4f(_color.r(),_color.g(),_color.b(),0.75);
  drawLine(_origin,_yawPos);
  drawLine(_yawPos,_shiftPos);
  drawLine(_shiftPos,_f.eye());
  glEnd();
}


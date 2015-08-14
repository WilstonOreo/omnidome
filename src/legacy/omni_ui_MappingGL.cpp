/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <omni/ui/MappingGL.h>

#include <QColor>
#include <QDebug>
#include <omni/Session.h>
#include <omni/Mapping.h>

#include <QMouseEvent>
#include <QImageReader>

#include <omni/util.hpp>
#include <omni/debug.hpp>

using namespace omni::ui;

MappingGL::MappingGL(QWidget *_parent) :
  GLView(_parent),
  border_(0.0),
  ignoreAspect_(false),
  interactive_(false),
  showCursor_(true),
  isPressed_(false),
  previewUpdated_(false),
  projTex_(nullptr),
  strokeBlendTex_(nullptr),
  frameBuffer_(nullptr),
  mapId_(0),
  leftOverDistance_(0)
{
  setFocusPolicy(Qt::StrongFocus);
}

void MappingGL::interactive(bool _interactive)
{
  interactive_ = _interactive;
  setMouseTracking(showCursor_ && interactive_);
}

void MappingGL::showCursor(bool _showCursor)
{
  showCursor_ = _showCursor;
  setMouseTracking(showCursor_ && interactive_);
}


MappingGL::~MappingGL()
{
}

void MappingGL::initialize(Session* _session, size_t _mapId, const view_set_type& _views)
{
  makeCurrent();
  mapId_ = _mapId;
  GLView::initialize(_session,_views);
  if (!frameBuffer_)
    initializeFrameBuffer();

  if (!strokeBlendTex_)
  {
    strokeBlendTex_.reset(new blend_texture_type());
    strokeBlendTex_->initialize(&mapping()->blendMask().strokeBuffer());
  }

  if (!testTexture())
    useTexture();
}

void MappingGL::free()
{
  GLView::free();
  frameBuffer_.reset(nullptr);
  projTex_.reset(nullptr);
  strokeBlendTex_.reset(nullptr);
}

void MappingGL::initializeGL()
{
  GLView::initializeGL();
}

void MappingGL::useTexture()
{
  if (!session() || !mapping()) return;
  makeCurrent();
  if (session()->mode() == Session::CANVAS_PROJECTORS)
  {
    if (mapping()->projectorImage().empty()) return;

    if (!projTex_)
    {
      projTex_.reset(new proj_texture_type());
    }
    projTex_->initialize(&mapping()->projectorImage());
    use_texture(*projTex_,[&]()
    {
      proj_texture_type::wrap(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
      proj_texture_type::min_mag(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR);
      proj_texture_type::mipmap();
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    });
  }
  else if (session()->mode() == Session::BLEND)
  {
    strokeBlendTex_->initialize(&mapping()->blendMask().strokeBuffer());
  }
  else
  {
    GLView::useTexture();
  }
}

void MappingGL::drawFrameBuffer() const
{
  if (!testTexture()) return;
  auto& _proj = mapping()->projector();
  proj::Frustum _f(_proj);
  draw_on_framebuffer(*frameBuffer_,[&]()
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_proj.fov(),mapping()->aspectRatio(),0.01, 100.0);

    // realize coordinates
    gluLookAt(
      _f.eye().x(),
      _f.eye().y(),
      _f.eye().z(),
      _f.eye().x() + _f.look_at().x(),
      _f.eye().y() + _f.look_at().y(),
      _f.eye().z() + _f.look_at().z(),
      _f.up().x(),
      _f.up().y(),
      _f.up().z()
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    use_texture(*testTexture(),[&]()
    {
      //  testTexture()->initialize();
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      drawCanvas();
      glEnable(GL_BLEND);
    });
  });
}

void MappingGL::drawPositioning() const
{
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  draw_with_framebuffer_texture(*frameBuffer_,[&]()
  {
    drawScreenRect();
  });
}

void MappingGL::resizeGL(int _w, int _h)
{
  GLView::resizeGL(_w,_h);
  update();
}


QRectF MappingGL::viewRect(bool _upsideDown) const
{
  if (!mapping())
  {
    return QRect(0,0,0,0);
  }
}

QRectF MappingGL::projRect() const
{
  Scalar _projAspect = Scalar(mapping()->width()) / mapping()->height();
  float _left = -0.5,_right = 0.5,_bottom = -0.5,_top = 0.5;
  if (_projAspect > 1.0)
  {
    _top *= _projAspect;
    _bottom *=  _projAspect;
  }
  else
  {
    _left /= _projAspect;
    _right /= _projAspect;
  }
  return QRectF(QPointF(_left,_top),QPointF(_right,_bottom));
}

QPointF MappingGL::toScreenPos(QPointF const& _pos) const
{
  QRectF&& _rect = viewRect();
  QPointF _p = QPointF(_pos.x() / width(),_pos.y() / height() ) - QPointF(0.5,0.5);

  return QPointF(_p.x() * _rect.width(),-_p.y() * _rect.height());
}

omni::Point2 MappingGL::pixelPos(Point2 const& _p) const
{
  if (!mapping()) return Point2(0,0);
  auto&& _s = toScreenPos(QPointF(_p.x(),_p.y()));
  return Point2(
           float(_s.x()+0.5)*mapping()->width(),
           float(_s.y()+0.5)*mapping()->height());
}


QPointF MappingGL::screenPos() const
{
  return toScreenPos(QPointF(mousePosition().x(),mousePosition().y()));
}


void MappingGL::drawBlendMask() const
{
  if (!mapping()) return;
  if (!blendShader()) return;

  auto& _mask = mapping()->blendMask();
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0,1.0,1.0,1.0);

  auto& _bs = *blendShader();
  use_shader(_bs,[&]()
  {
    _bs("top",_mask.top_width())
    ("left",_mask.left_width())
    ("right",_mask.right_width())
    ("bottom",_mask.bottom_width())
    ("gamma",_mask.gamma())
    ("mask",GLfloat(-1.0));
    drawWarpPatch();
    drawScreenRect();
  });

  glDisable(GL_BLEND);
  glColor4f(0.0,0.0,0.0,1.0);

  glBegin(GL_QUADS);
  float _b = 4.0;
  glVertex2f(-0.5,-0.5 - _b);
  glVertex2f(0.5-_b,-0.5);
  glVertex2f(0.5-_b,0.5);
  glVertex2f(-0.5,0.5 + _b);

  glVertex2f(-0.5 + _b,-0.5 - _b);
  glVertex2f(0.5,-0.5);
  glVertex2f(0.5,0.5);
  glVertex2f(-0.5 + _b,0.5 + _b);


  glVertex2f(-0.5 + _b,0.5 + _b);
  glVertex2f(0.5 - _b,0.5 + _b);
  glVertex2f(0.5 - _b,0.5);
  glVertex2f(-0.5 + _b,0.5);

  glVertex2f(-0.5 + _b,-0.5);
  glVertex2f(0.5 - _b,-0.5);
  glVertex2f(0.5 - _b,-0.5 - _b);
  glVertex2f(-0.5 + _b,-0.5 - _b);

  glEnd();

  glEnable(GL_BLEND);
  use_texture(*strokeBlendTex_,[&]()
  {
    glColor4f(0.0,0.0,0.0,1.0);
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    drawScreenRect();
  });


  drawCursor();

  if (!ignoreAspect())
  {
    auto& _color = mapping()->color();
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT,GL_LINE);
    glPolygonMode(GL_BACK,GL_LINE);
    glColor4f(_color.r(),_color.g(),_color.b(),1.0);
    drawScreenRect();
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_FILL);
  }
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void MappingGL::drawWarpGrid() const
{
  glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  auto& _warpGrid = mapping()->warpGrid();
  auto& _color = mapping()->color();
  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);
  glDisable(GL_BLEND);
  draw_with_framebuffer_texture(*frameBuffer_,[&]()
  {
    glDisable(GL_LIGHTING);
    drawWarpPatch();
  });
  glEnable(GL_BLEND);

  glPolygonMode(GL_FRONT,GL_LINE);
  glPolygonMode(GL_BACK,GL_LINE);
  glColor4f(1.0,1.0,1.0,0.5);
  glLineWidth(2.0);
  drawWarpPatch();

  if (!ignoreAspect())
  {
    glColor4f(_color.r(),_color.g(),_color.b(),1.0);
    drawScreenRect();
  }
  glPolygonMode(GL_FRONT,GL_FILL);
  glPolygonMode(GL_BACK,GL_FILL);

  if (interactive() || showCursor())
  {
    constexpr size_t _numVertices = 16;
    const float _radius = 0.025;
    std::array<Point2,_numVertices> _circlePoints;
    auto&& _rect = projRect();
    util::for_each_circle_point(_numVertices,_radius,[&](size_t i, const Point2& _p)
    {
      _circlePoints[i] = Point2(_p.x() * _rect.width(),_p.y() * _rect.height());
    });

    for (auto& _point : _warpGrid.points())
    {
      glBegin(GL_LINE_LOOP);
      glColor4f(1.0,1.0,1.0,0.5);
      for (auto& _p : _circlePoints)
        glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
      glEnd();
      if (_point.selected())
      {
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(_color.r(),_color.g(),_color.b(),0.5);
        glVertex2f(_point.x(),_point.y());
        for (auto& _p : _circlePoints)
        {
          glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
        }
        auto& _p = _circlePoints[0];
        glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
        glEnd();
      }
    }
  }

}

void MappingGL::drawScreenRect() const
{
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex2f(-0.5,-0.5);
  glTexCoord2f(1.0, 0.0);
  glVertex2f(0.5,-0.5);
  glTexCoord2f(1.0, 1.0);
  glVertex2f(0.5,0.5);
  glTexCoord2f(0.0, 1.0);
  glVertex2f(-0.5,0.5);
  glEnd();
}

void MappingGL::drawWarpPatch() const
{
  if (!mapping()) return;
  auto& _warpGrid = mapping()->warpGrid();
  for (int y = 0; y < _warpGrid.vertical()-1; ++y)
  {
    glBegin(GL_QUAD_STRIP);
    for (int x = 0; x < _warpGrid.horizontal(); ++x)
    {
      auto& _p0 = *_warpGrid.getPoint(x,y);
      auto& _py = *_warpGrid.getPoint(x,y+1);
      auto _texCoord0 = _warpGrid.getTexCoord(x,y);
      auto _texCoordY = _warpGrid.getTexCoord(x,y+1);
      glTexCoord2f(_texCoord0.x(), _texCoord0.y());
      glVertex2f(_p0.x(),_p0.y());
      glTexCoord2f(_texCoordY.x(), _texCoordY.y());
      glVertex2f(_py.x(),_py.y());
    }
    glEnd();
  }
}

void MappingGL::initializeFrameBuffer()
{
  if (!mapping()) return;
  makeCurrent();
  frameBuffer_.reset(new FrameBufferTexture(mapping()->width(),mapping()->height()));
  drawFrameBuffer();
}


void MappingGL::drawPreview()
{
  use_texture(*projTex_,[&]()
  {
    proj_texture_type::wrap(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    proj_texture_type::min_mag(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR);
    proj_texture_type::mipmap();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    drawScreenRect();
  });
}

void MappingGL::paintGL()
{
  if (!session()) return;

  auto _mode = session()->mode();
  auto&& _rect = viewRect(_mode == Session::CANVAS_PROJECTORS);
  if ((_mode == Session::POSITIONING || _mode == Session::SIMULATE) && !!frameBuffer_)
  {
    drawFrameBuffer();
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //   glDisable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(_rect.left(),_rect.right(),_rect.top(),_rect.bottom());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  auto& _color = mapping()->color();

  switch (_mode)
  {
  case Session::CANVAS_PROJECTORS:
    drawPreview();
    break;
  case Session::POSITIONING:
    drawPositioning();
    break;
  case Session::WARP:
    drawWarpGrid();
    break;
  case Session::BLEND:
    if (interactive() || showCursor())
      strokeBlendTex_->initialize(&mapping()->blendMask().strokeBuffer());
    if (whiteBlendMask())
    {
      glColor4f(1.0,1.0,1.0,1.0);
    }
    else
    {
      glColor4f(_color.r(),_color.g(),_color.b(),1.0);
    }

    drawWarpPatch();
    drawBlendMask();
    break;
  case Session::SIMULATE:
  case Session::EXPORT:
    draw_with_framebuffer_texture(*frameBuffer_,[&]()
    {
      glDisable(GL_LIGHTING);
      glColor4f(1.0,1.0,1.0,1.0);
      glDisable(GL_BLEND);
      drawWarpPatch();
      glEnable(GL_BLEND);
    });
    drawBlendMask();

  default :
    break;
  }

  swapBuffers();
}

void MappingGL::drawCursor() const
{
  if (!session() || !mapping()) return;
  auto _mode = session()->mode();
  if (_mode == Session::BLEND)
  {
    auto& _mask = mapping()->blendMask();
    /// Draw Blend cursor
    if (interactive() || showCursor())
    {
      QPointF _pos = interactive() ? screenPos() : cursorPosition();

      glDisable(GL_LINE_SMOOTH);

      auto&& _rect = projRect();
      glColor4f(1.0,1.0,1.0,1.0);
      glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
      glBegin(GL_LINE_LOOP);
      float _r = _mask.brush_size() * 0.25 / mapping()->width();;
      util::for_each_circle_point(24,_r,[&](size_t _i, const Point2& _p)
      {
        glVertex2f(_p.x() + _pos.x(),_p.y() * (_rect.height()/_rect.width()) + _pos.y());
      });
      glEnd();
      glEnable(GL_LINE_SMOOTH);
    }
  }
}


void MappingGL::mouseMoveEvent(QMouseEvent *event)
{
  auto&& _rect = viewRect();
  float dx = float(event->x() - mousePosition().x()) / width() * _rect.width();
  float dy = float(event->y() - mousePosition().y()) / height() * _rect.height();
  if (!session() || (!interactive() && !showCursor())) return;

  auto _mode = session()->mode();
  if (_mode == Session::WARP)
  {
    if (isPressed_)
    {
      auto&& _selectedPoints = mapping()->warpGrid().getSelected();
      for (auto& _selected : _selectedPoints)
      {
        _selected->pos() += Point2(dx,-dy);
      }
      emit command();
    }
  }
  else if (_mode == Session::BLEND)
  {
    makeCurrent();

    if (isPressed_)
    {
      leftOverDistance_ = drawLine(
                            Point2(mousePosition().x(),mousePosition().y()),
                            Point2(event->x(),event->y()),leftOverDistance_);
      strokeBlendTex_->initialize();
      emit command();
    }
  }

  mousePosition() = event->pos();
  if (interactive())
  {
    cursorPosition() = screenPos();
  }
  drawCursor();

  for (auto& _view : views())
  {
    _view->cursorPosition(cursorPosition());
    _view->update();
  }
  update();
}

void MappingGL::mousePressEvent(QMouseEvent *event)
{
  if (!session() || !interactive()) return;
  mousePosition() = event->pos();
  auto&& _newPos = screenPos();

  isPressed_ = true;

  auto _mode = session()->mode();
  if (_mode == Session::WARP)
  {
    auto& _warpGrid = mapping()->warpGrid();
    auto&& _selectedPoints = _warpGrid.getSelected();
    auto _p = _warpGrid.selectNearest(Point2(_newPos.x(),_newPos.y()));
    if (!_p) return;
    if (!(event->modifiers() & Qt::ControlModifier))
    {
      _warpGrid.selectNone();
    }
    _p->selected(!_p->selected() || (_selectedPoints.size() > 1));
  }
  else if (_mode == Session::BLEND)
  {
    bool _inv = mapping()->blendMask().invert_brush();
    if (interactive())
    {
      cursorPosition() = screenPos();

      if (event->button() == Qt::RightButton)
      {
        mapping()->blendMask().invert_brush(!_inv);
      }
    }
    strokeBlendTex_->initialize();
    drawBrush(pixelPos(Point2(event->x(),event->y())));
    leftOverDistance_ = 0.0;
  }

  update();
  for (auto& _view : views())
  {
    _view->cursorPosition(cursorPosition());
    _view->update();
  }
}

void MappingGL::drawBrush(Point2 const& _p)
{
  auto& _mask = mapping()->blendMask();
  _mask.stamp(_p);
  makeCurrent();
  strokeBlendTex_->initialize();
  update();
}

void MappingGL::mouseReleaseEvent(QMouseEvent *event)
{
  if (!session()) return;
  isPressed_ = false;
  auto _mode = session()->mode();

  if (_mode == Session::BLEND)
  {
    makeCurrent();
    leftOverDistance_ = 0.0;
    strokeBlendTex_->initialize();
    update();
    for (auto& _view : views())
    {
      _view->useTexture();
    }

    bool _inv = mapping()->blendMask().invert_brush();
    if (event->button() == Qt::RightButton)
    {
      mapping()->blendMask().invert_brush(!_inv);
    }
  }
}

void MappingGL::wheelEvent(QWheelEvent* _event)
{
  if (!session()) return;
  auto _mode = session()->mode();

  if (_mode == Session::BLEND)
  {
    mapping()->blendMask().changeBrushSize(_event->delta() / 5.0);
    update();
    for (auto& _view : views())
    {
      _view->update();
    }
  }
}
void MappingGL::keyPressEvent(QKeyEvent* event)
{
  if (!session() || !interactive()) return;

  auto _mode = session()->mode();

  if (_mode == Session::WARP)
  {
    auto&& _selectedPoints = mapping()->warpGrid().getSelected();
    auto&& _rect = viewRect();
    float dx = 1.0 / width() * _rect.width();
    float dy = 1.0 / height() * _rect.height();

    for (auto& _selected : _selectedPoints)
    {
      switch( event->key() )
      {
      case Qt::Key_Up:
        _selected->y() += dy;
        break;
      case Qt::Key_Left:
        _selected->x() -= dx;
        break;
      case Qt::Key_Down:
        _selected->y() -= dy;
        break;
      case Qt::Key_Right:
        _selected->x() += dx;
        break;
      }
    }
    update();
    for (auto& _view : views())
    {
      _view->update();
    }
  }
  else if (_mode == Session::BLEND)
  {
    switch( event->key() )
    {
    case Qt::Key_Left:
      mapping()->blendMask().changeBrushSize(-8.0);
      break;
    case Qt::Key_Right:
      mapping()->blendMask().changeBrushSize(8.0);
      break;
    }

    update();
    for (auto& _view : views())
    {
      _view->update();
    }
  }
  QGLWidget::keyPressEvent(event);
}

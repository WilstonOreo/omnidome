#include <omni/ui/TuningGLView.h>

#include <QDebug>
#include <QResizeEvent>
#include <QMatrix4x4>
#include <omni/visual/util.h>
#include <omni/visual/Rectangle.h>

namespace omni
{
  namespace ui
  {
    TuningGLView::TuningGLView(QWidget* _parent) :
      GLView(_parent),
      cursorPosition_(0.0,0.0)
    {
      setViewOnly(false);
    }

    TuningGLView::~TuningGLView()
    {
      destroy();
    }

    omni::proj::Tuning* TuningGLView::tuning()
    {
      return this->session_->session().tunings()[index_];
    }

    omni::proj::Tuning const* TuningGLView::tuning() const
    {
      return this->session_->session().tunings()[index_];
    }

    void TuningGLView::setTuningIndex(int _index)
    {
      index_=_index;
      auto* _tuning = tuning();

      if (!_tuning) return;
      tuning_.reset(new visual::Tuning(*_tuning));
      tuning_->update();
    }

    bool TuningGLView::keepAspectRatio() const
    {
      return keepAspectRatio_;
    }

    void TuningGLView::setKeepAspectRatio(bool _keepAspectRatio)
    {
      keepAspectRatio_ = _keepAspectRatio;
      sizePolicy().setHeightForWidth(keepAspectRatio_);
    }

    bool TuningGLView::viewOnly() const
    {
      return viewOnly_;
    }

    void TuningGLView::setViewOnly(bool _viewOnly)
    {
      viewOnly_ = _viewOnly;
      setMouseTracking(showCursor_ && !viewOnly_);
    }

    void TuningGLView::setShowCursor(bool _showCursor)
    {
      showCursor_ = _showCursor;
      setMouseTracking(showCursor_ && !viewOnly_);
    }
    
    bool TuningGLView::showCursor() const
    {
      return showCursor_;
    }

    bool TuningGLView::isDrawingEnabled() const
    {
      return drawingEnabled_;
    }

    void TuningGLView::setDrawingEnabled(bool _drawingEnabled)
    {
      drawingEnabled_ = _drawingEnabled;
    }

    bool TuningGLView::isFullscreen() const
    {
      return fullscreen_;
    }

    float TuningGLView::border() const
    {
      return border_;
    }

    void TuningGLView::setBorder(float _border)
    {
      border_ = _border;
      update();
    }

    /// Show different content for different session modes
    void TuningGLView::sessionModeChange()
    {
    }

    void TuningGLView::setFullscreen(Screen const& _screen)
    {
      setWindowFlags( Qt::CustomizeWindowHint );
      setWindowFlags(Qt::FramelessWindowHint);
      fullscreen_ = true;
      showFullScreen();
    }

    void TuningGLView::mouseMoveEvent(QMouseEvent *event)
    {
      if (!session() || !tuning_ || (viewOnly() && !showCursor_)) return;

      auto&& _rect = viewRect();
      float dx = float(event->x() - mousePosition().x()) / width() * _rect.width();
      float dy = float(event->y() - mousePosition().y()) / height() * _rect.height();

      auto _mode = session()->mode();

      if (mouseDown_)
      {
        auto& _warpGrid = tuning()->warpGrid();

        // Manipulate selected warp grid point, when mouse button is pressed
        if (_mode == Session::Mode::WARP)
        {
          auto&& _selectedPoints = _warpGrid.getSelected();
          for (auto& _selected : _selectedPoints)
          {
            _selected->pos() += QPointF(dx,-dy);
          }
        }
        else if (_mode == Session::Mode::BLEND)
        {
          leftOverDistance_ = tuning()->blendMask().drawLine(pixelPos(mousePosition_),pixelPos(event->pos()),leftOverDistance_);
          tuning_->update();
        }
      }

      mousePosition_ = event->pos();
      /*if (!viewOnly())
      {
        //cursorPosition() = screenPos();
      }
      */

      update();
    }

    void TuningGLView::mousePressEvent(QMouseEvent *event)
    {
      makeCurrent();
      QOpenGLWidget::mousePressEvent(event);
      if (!session() || !tuning_ || viewOnly()) return;

      this->mousePosition_ = event->pos();
      auto&& _newPos = screenPos(this->mousePosition_);
      mouseDown_ = true;

      auto _mode = session()->mode();
      if (_mode == Session::Mode::WARP)
      {
        auto& _warpGrid = tuning()->warpGrid();

        auto&& _selectedPoints = _warpGrid.getSelected();
        auto _p = _warpGrid.selectNearest(QPointF(_newPos.x(),_newPos.y()));
        if (!_p) return;

        // Deselect points if ctrl key is not pressed
        if (!(event->modifiers() & Qt::ControlModifier))
        {
          _warpGrid.selectNone();
        }
        // Select point if it is not selected already or
        // number of selected point is larger than 1
        _p->setSelected(!_p->selected() || (_selectedPoints.size() > 1));
      }
      else if (_mode == Session::Mode::BLEND)
      {
        bool _inv = tuning()->blendMask().brush().invert();

        // Invert brush on right click
        if (event->button() == Qt::RightButton)
        {
          tuning()->blendMask().brush().setInvert(!_inv);
        }

        auto& _blendMask = tuning()->blendMask();
        _blendMask.stamp(pixelPos(event->pos()));
        tuning_->update();

        leftOverDistance_ = 0.0;
      }

      update();
    }

    void TuningGLView::mouseReleaseEvent(QMouseEvent *event)
    {
      if (!session() || !tuning_ || viewOnly()) return;

      mouseDown_ = false;
      makeCurrent();
      auto _mode = session()->mode();
      if (_mode == Session::Mode::BLEND)
      {
        leftOverDistance_ = 0.0;

        // Invert brush on right click
        bool _inv = tuning()->blendMask().brush().invert();
        if (event->button() == Qt::RightButton)
        {
          tuning()->blendMask().brush().setInvert(!_inv);
        }
      }
      this->mousePosition_ = event->pos();
      update();
    }

    void TuningGLView::wheelEvent(QWheelEvent* event)
    {
      if (!session()) return;
      auto _mode = session()->mode();

      if (_mode == Session::Mode::BLEND)
      {
        auto& _blendMask = tuning()->blendMask();
        _blendMask.brush().changeSize(event->delta() / 5.0);
        update();
      }
    }

    void TuningGLView::keyPressEvent(QKeyEvent* event)
    {
    }
      
    void TuningGLView::destroy()
    {
      aboutToBeDestroyed_ = true;
    }

    bool TuningGLView::initialize()
    {
      if (context())
      { 
        connect(context(),SIGNAL(aboutToBeDestroyed()),this,SLOT(destroy()));
      }

      return context() != nullptr;
    }

    QRectF TuningGLView::viewRect() const
    {
      float _projAspect = float(tuning()->width()) / tuning()->height();
      float _viewAspect = float(width()) / height();
      float b = border_ * 0.5;
      float _left = -0.5 - b,_right = 0.5 + b,_bottom = -0.5 - b,_top = 0.5 + b;

      if (keepAspectRatio())
      {
        if (_projAspect > _viewAspect)
        {
          _top *= _projAspect / _viewAspect;
          _bottom *=  _projAspect / _viewAspect;
        }
        else
        {
          _left *= _viewAspect / _projAspect;
          _right *= _viewAspect / _projAspect;
        }
      }
      return QRectF(QPointF(_left,_top),QPointF(_right,_bottom));
    }

    QPointF TuningGLView::screenPos(QPointF const& _pos) const
    {
      QRectF&& _rect = viewRect();
      QPointF _p = QPointF(_pos.x() / width() - 0.5,_pos.y() / height() - 0.5);
      return QPointF(_p.x() * _rect.width(),-_p.y() * _rect.height());
    }

    QPointF TuningGLView::pixelPos(QPointF const& _pos) const
    {
      if (!tuning()) return QPointF(0,0);
      auto&& _s = screenPos(QPointF(_pos.x(),_pos.y()));
      return QPointF(
           float(_s.x()+0.5)*tuning()->width(),
           float(_s.y()+0.5)*tuning()->height());
    }

    void TuningGLView::drawCanvas()
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMultMatrixf(tuning()->projector().projectionMatrix().constData());
      glMatrixMode(GL_MODELVIEW);

      glLoadIdentity();

      glDisable(GL_LIGHTING);
      session_->drawCanvas();
    }

    /// Update warp buffer which contains image of projector perspective
    void TuningGLView::updateWarpBuffer()
    {
      // If tuning size has changed, reset warpGrid framebuffer
      if (warpGridBuffer_)
      {
        if ((warpGridBuffer_->width() != tuning()->width()) ||
            (warpGridBuffer_->height() != tuning()->height()))
          warpGridBuffer_.reset();
      }

      // If warp grid framebuffer is empty, make a new one
      if (!warpGridBuffer_)
      {
        warpGridBuffer_.reset(new QOpenGLFramebufferObject(
                                tuning()->width(),
                                tuning()->height()));
      }

      // Draw projector's perspective on framebuffer texture
      visual::with_current_context([this](QOpenGLFunctions& _)
      {
        _.glEnable(GL_TEXTURE_2D);
        _.glDisable(GL_LIGHTING);
        visual::draw_on_framebuffer(warpGridBuffer_,
                                    [&](QOpenGLFunctions& _) // Projection Operation
        {
          glMultMatrixf(tuning()->projector().projectionMatrix().constData());
        },
        [&](QOpenGLFunctions& _) // Model View Operation
        {
          _.glClearColor(0.0,0.0,0.0,1.0);
          session_->drawCanvas();
        });
      });
    }
      
    template<typename F> 
    void TuningGLView::drawOnSurface(F f)    
    {
        makeCurrent();
        visual::with_current_context([&](QOpenGLFunctions& _)
        {
          visual::viewport(this);
          _.glClearColor(0.0,0.0,0.0,1.0);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          QMatrix4x4 _m;
          QRectF&& _rect = viewRect();
          _m.ortho(_rect.left(),_rect.right(),_rect.top(),_rect.bottom(),-1.0,1.0);
          glMultMatrixf(_m.constData());

          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          f(_);
        });
    }

    void TuningGLView::drawWarpGrid()
    {
      updateWarpBuffer();

      drawOnSurface([&](QOpenGLFunctions& _)
      {
        _.glBindTexture(GL_TEXTURE_2D, warpGridBuffer_->texture());
        _.glDisable(GL_LIGHTING);
        _.glEnable(GL_BLEND);
        tuning_->drawWarpGrid();

        _.glBindTexture(GL_TEXTURE_2D, 0.0);

        drawScreenBorder();
      });
    }

    void TuningGLView::drawBlendMask()
    {
      auto _blendMode = session()->blendMode();

      if (_blendMode == Session::BlendMode::INPUT)
      {
        updateWarpBuffer();
      }

      drawOnSurface([&](QOpenGLFunctions& _)
      {
        _.glDisable(GL_LIGHTING);
        _.glEnable(GL_BLEND);
   
        switch (_blendMode) 
        {
        case Session::BlendMode::COLOR:
        {
          // Draw blend mask with color of tuning
          _.glDisable(GL_TEXTURE_2D);
          auto _color = tuning()->color();
          glColor4f(_color.redF(),_color.greenF(),_color.blueF(),1.0);
          tuning_->drawBlendMask();
          break;
        }
        case Session::BlendMode::WHITE:
        {
          // Draw white blend mask 
          _.glDisable(GL_TEXTURE_2D);
          glColor4f(1.0,1.0,1.0,1.0);
          tuning_->drawBlendMask();
          break;
        }
        case Session::BlendMode::INPUT:
        {
          // Draw blend mask with input attached
          _.glEnable(GL_TEXTURE_2D);
          _.glBindTexture(GL_TEXTURE_2D, warpGridBuffer_->texture());
          tuning_->drawBlendMask();
          _.glBindTexture(GL_TEXTURE_2D, 0.0);
          break;
        }
        };
 
        drawScreenBorder();

        if (showCursor_ && !viewOnly_)
          tuning_->drawCursor(screenPos(mousePosition_));
      });
    }
      
    void TuningGLView::drawScreenBorder()
    {
        // Draw screen border rectangle on top
        if (!viewOnly())
        {
          glPolygonMode(GL_FRONT,GL_LINE);
          glPolygonMode(GL_BACK,GL_LINE);
          auto _color = tuning()->color();
          glColor3f(_color.redF(),_color.greenF(),_color.blueF());
          visual::Rectangle::draw();
          glPolygonMode(GL_FRONT,GL_FILL);
          glPolygonMode(GL_BACK,GL_FILL);
        }
    }

    void TuningGLView::drawTestCard()
    {
      drawOnSurface([&](QOpenGLFunctions& _)
      {
        tuning_->drawTestCard(index_+1);
      });
    }

    void TuningGLView::paintGL()
    {
      if (!isVisible() || !context() || aboutToBeDestroyed_) return;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (!tuning_ || !session_) return;

      if (!isDrawingEnabled())
        if (frameBuffer_)
        {
          frameBuffer_->bind();
          glViewport(0, 0, (GLint)frameBuffer_->width(), (GLint)frameBuffer_->height());
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

      glDisable(GL_BLEND);

      session_->update();

      if (!tuning_->initialized())
        tuning_->update();

      glPushAttrib(GL_ALL_ATTRIB_BITS);
      switch (session()->mode())
      {
      case Session::Mode::SCREENSETUP:
        drawTestCard();
        break;
      case Session::Mode::PROJECTIONSETUP:
        if (!session()->hasOutput())
          drawTestCard();
        else
          drawCanvas();
        break;
      case Session::Mode::WARP:
        drawWarpGrid();
        break;
      case Session::Mode::BLEND:
        drawBlendMask();
        break;
      case Session::Mode::EXPORT:
        break;
      }
      glPopAttrib();

      if (!isDrawingEnabled())
      {
        if (frameBuffer_)
        {
          frameBuffer_->release();
          makeCurrent();

          visual::viewport(this);
          glBindTexture(GL_TEXTURE_2D, frameBuffer_->texture());

          /// Setup orthogonal projection
          glMatrixMode(GL_PROJECTION);
          {
            glLoadIdentity();
            QMatrix4x4 _m;
            _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
            glMultMatrixf(_m.constData());
          }
          glDisable( GL_CULL_FACE );
          glEnable(GL_TEXTURE_2D);
          glMatrixMode(GL_MODELVIEW);

          glLoadIdentity();
          glActiveTexture(GL_TEXTURE0);

          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          glBegin(GL_QUADS);
          {
            glTexCoord2f(0.0f,0.0f);
            glVertex2f(-0.5f,-0.5f);
            glTexCoord2f(1.0f,0.0f);
            glVertex2f(0.5f,-0.5f);
            glTexCoord2f(1.0f,1.0f);
            glVertex2f(0.5f,0.5f);
            glTexCoord2f(0.0f,1.0f);
            glVertex2f(-0.5f,0.5f);
          }
          glEnd();

          glBindTexture(GL_TEXTURE_2D, 0);

          glEnable(GL_BLEND);

        }
      }
    }
  }
}

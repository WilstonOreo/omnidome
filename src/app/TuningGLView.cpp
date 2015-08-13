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
      setFocusPolicy(Qt::StrongFocus);
      setMouseTracking(showCursor_ && !viewOnly_);
    }

    TuningGLView::~TuningGLView()
    {
      if (initialized() && context())
        free();
    }
      
    void TuningGLView::free()
    {
      makeCurrent();
      tuning_->free();
      tuning_.reset();
      session_->free();
      session_.reset();
      frameBuffer_.reset();
      warpGridBuffer_.reset();
      destroy();
      doneCurrent();
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

    float TuningGLView::border() const
    {
      return border_;
    }
      
    void TuningGLView::setChildViews(std::set<TuningGLView*> const& _childViews)
    {
      childViews_ = _childViews;
    }

    std::set<TuningGLView*> TuningGLView::childViews() const
    {
      return childViews_;
    }

    void TuningGLView::updateWithChildViews(bool _update) 
    {
      updateContext(_update);
      for (auto& _childView : childViews_)
      {
        _childView->updateContext(_update);
      }
    }
      
    void TuningGLView::updateWithChildViews(QRect const& _rect)
    {
      tuning_->setBlendTextureUpdateRect(_rect);
      tuning_->update();
      for (auto& _childView : childViews_)
      {
        if (_childView->tuning() == tuning())
        {
          _childView->updateWithChildViews(_rect);
        }
      }
      update();
    }

    void TuningGLView::updateContext(bool _update)
    {
      if (_update)
        tuning_->update();
      update();
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
          updateWithChildViews(false);
        }
        else if (_mode == Session::Mode::BLEND)
        {
          auto _from = pixelPos(mousePosition_).toPoint();
          auto _to = pixelPos(event->pos()).toPoint();
          leftOverDistance_ = tuning()->blendMask().drawLine(_from,_to,leftOverDistance_); 
          mousePosition_ = event->pos();
          cursorPosition_ = screenPos(mousePosition_);
          for (auto& _childView : childViews_)
          {
            _childView->cursorPosition_ = cursorPosition_;
          }
          updateWithChildViews(QRect(_from,_to));
        }
      }

      mousePosition_ = event->pos();
      cursorPosition_ = screenPos(mousePosition_);

      if (!mouseDown_)
      {
        for (auto& _childView : childViews_)
        {
          _childView->cursorPosition_ = cursorPosition_;
          if (_childView->showCursor())
            _childView->update();
        }
        update();
      } 
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
        updateWithChildViews(false);
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
        updateWithChildViews();

        leftOverDistance_ = 0.0;
      }
      
      for (auto& _childView : childViews_)
      {
        _childView->cursorPosition_ = cursorPosition_;
        if (_childView->showCursor())
          _childView->update();
      }
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
      updateWithChildViews();
    }

    void TuningGLView::wheelEvent(QWheelEvent* event)
    {
      if (!session()) return;
      auto _mode = session()->mode();

      if (_mode == Session::Mode::BLEND)
      {
        auto& _blendMask = tuning()->blendMask();
        _blendMask.brush().changeSize(event->delta() / 5.0);
        updateWithChildViews(false);
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
        qDebug() << "connect(context(),SIGNAL(aboutToBeDestroyed()),this,SLOT(free()));";
        connect(context(),SIGNAL(aboutToBeDestroyed()),this,SLOT(free()));
        //connect(context(),SIGNAL(aboutToBeDestroyed()),this,SLOT(destroy()));
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

        _.glBindTexture(GL_TEXTURE_2D, 0);

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

        if (showCursor_ || !viewOnly_)
          tuning_->drawCursor(cursorPosition_);
      });
    }
      
    void TuningGLView::drawExportView()
    {
      if (!session()->hasOutput())
      {
        drawTestCard();
        return;
      }
      updateWarpBuffer();

      drawOnSurface([&](QOpenGLFunctions& _)
      {
        _.glDisable(GL_LIGHTING);
        _.glEnable(GL_BLEND);
   
        // Draw blend mask with input attached
        _.glEnable(GL_TEXTURE_2D);
        _.glBindTexture(GL_TEXTURE_2D, warpGridBuffer_->texture());
        tuning_->drawBlendMask();
        _.glBindTexture(GL_TEXTURE_2D, 0.0);
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

      if (!tuning_ || !session_) return;

      session_->update();

      if (!tuning_->initialized()) 
        tuning_->update();

      makeCurrent();
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      visual::viewport(this);
      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        _.glClearColor(0.0,0.0,0.0,1.0);
      });
        
      if (!session()->hasOutput())
      {
        drawTestCard();
        return;
      }

      switch (session()->mode())
      {
      case Session::Mode::SCREENSETUP:
        drawTestCard();
        break;
      case Session::Mode::PROJECTIONSETUP:
        drawCanvas();
      case Session::Mode::WARP:
        drawWarpGrid();
        break;
      case Session::Mode::BLEND:
        drawBlendMask();
        break;
      case Session::Mode::EXPORT:
        drawExportView();  
        break;
      }
      glPopAttrib();
    }
  }
}

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
      mixin::TuningFromIndex<TuningGLView>(*this),
      cursorPosition_(0.0,0.0)
    {
      setViewOnly(false);
      setFocusPolicy(Qt::StrongFocus);
      setMouseTracking(showCursor_ && !viewOnly_);
    }

    TuningGLView::~TuningGLView()
    {
//        if (!aboutToBeDestroyed_) free();
    }

    void TuningGLView::free()
    {
      if (!initialized() || !context()) return;
      if (!context()->isValid()) return;

      makeCurrent();
      vizTuning_->free();
      vizTuning_.reset();
      session_->free();
      session_.reset();
      frameBuffer_.reset();
      warpGridBuffer_.reset();
      destroy();
      doneCurrent();
    }

    void TuningGLView::setTuningIndex(int _index)
    {
      setIndex(_index);
      auto* _tuning = tuning();

      if (!_tuning) return;

      // Make new visualizer
      vizTuning_.reset(new visual::Tuning(*_tuning));
      vizTuning_->update();
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
      vizTuning_->setBlendTextureUpdateRect(_rect);
      vizTuning_->update();
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
        vizTuning_->update();
      update();
    }

    void TuningGLView::setBorder(float _border)
    {
      border_ = _border;
      update();
    }

    void TuningGLView::mouseMoveEvent(QMouseEvent *event)
    {
      if (!session() || !vizTuning_ || (viewOnly() && !showCursor_)) return;

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

          updateWithChildViews(true);
        }
        else if (_mode == Session::Mode::BLEND)
        {
          auto _from = pixelPos(mousePosition_).toPoint();
          auto _to = pixelPos(event->pos()).toPoint();
          if (!(event->modifiers() & Qt::ShiftModifier)) {
              leftOverDistance_ = tuning()->blendMask().drawLine(_from,_to,leftOverDistance_);
              lastStrokePos_ = _from;
          }

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
      if (!session() || !vizTuning_ || viewOnly()) return;

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
        lastStrokePos_ = pixelPos(mousePosition_).toPoint();

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
      if (!session() || !vizTuning_ || viewOnly()) return;

      mouseDown_ = false;
      makeCurrent();
      auto _mode = session()->mode();

      if (_mode == Session::Mode::BLEND)
      {
        if (event->modifiers() & Qt::ShiftModifier) {
              auto _from = lastStrokePos_;
              auto _to = pixelPos(event->pos()).toPoint();
              tuning()->blendMask().drawLine(_from,_to,leftOverDistance_);
        }
        leftOverDistance_ = 0.0;

        // Invert brush on right click
        bool _inv = tuning()->blendMask().brush().invert();
        if (event->button() == Qt::RightButton)
        {
          tuning()->blendMask().brush().setInvert(!_inv);
        }
      }
      this->mousePosition_ = event->pos();
      this->cursorPosition_ = screenPos(this->mousePosition_);

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
        const float _distance = 0.005;
        auto _moveWarpPoint = [&](float _x, float _y) {
            auto& _warpGrid = tuning()->warpGrid();
            auto&& _selectedPoints = _warpGrid.getSelected();

            for (auto& _p : _selectedPoints) {
                _p->pos() += QPointF(_x,_y);
            }
            updateWithChildViews();
        };

        switch (event->key()) {
            case Qt::Key_Down:
                _moveWarpPoint(0.0,_distance);
            break;
            case Qt::Key_Up:
                _moveWarpPoint(0.0,-_distance);
            break;
            case Qt::Key_Left:
                _moveWarpPoint(-_distance,0.0);
            break;
            case Qt::Key_Right:
                _moveWarpPoint(_distance,0.0);
            break;
        }
    }

    void TuningGLView::destroy()
    {
      aboutToBeDestroyed_ = true;
    }

    bool TuningGLView::initialize()
    {
      if (context())
      {
        connect(context(),SIGNAL(aboutToBeDestroyed()),this,SLOT(free()));
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

      visual::with_current_context([this](QOpenGLFunctions& _)
      {
          _.glDisable(GL_LIGHTING);
          _.glDisable(GL_CULL_FACE);
          _.glEnable(GL_DEPTH_TEST);
          session_->drawCanvas(mapping::OutputMode::MAPPED_INPUT,tuning()->outputDisabled() && viewOnly());
          _.glDisable(GL_DEPTH_TEST);
      });
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
        warpGridBuffer_->setAttachment(QOpenGLFramebufferObject::Depth);
      }
      vizTuning_->updateWarpGrid();

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
          _.glClear(GL_DEPTH_BUFFER_BIT);

        _.glEnable(GL_DEPTH_TEST);
        _.glDepthFunc(GL_LEQUAL);
        _.glEnable(GL_BLEND);
        _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        _.glEnable(GL_LINE_SMOOTH);
        _.glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        _.glEnable(GL_POINT_SMOOTH);
        _.glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        _.glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glPolygonMode(GL_FRONT,GL_FILL);
        glPolygonMode(GL_BACK,GL_FILL);
        _.glEnable(GL_NORMALIZE);
        // fix outlines z-fighting with quads
        _.glPolygonOffset(1, 1);

          _.glEnable(GL_DEPTH_TEST);
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
      drawOutput(session()->blendSettings().showInWarpMode() ? 1.0 : 0.0 /* zero blend mask opacity */);

      drawOnSurface([&](QOpenGLFunctions& _)
      {
        vizTuning_->drawWarpGrid();
      });
    }

    void TuningGLView::drawBlendMask()
    {
      auto& _blendSettings = session()->blendSettings();
      auto _colorMode = _blendSettings.colorMode();
      float _inputOpacity = _blendSettings.inputOpacity();
      QColor _color = tuning()->color();
        if (_colorMode == BlendSettings::ColorMode::WHITE) {
            _color = Qt::white;
        }
        drawOutput(1.0,_inputOpacity,_color);

        glDisable(GL_DEPTH_TEST);
/*
        ///@todo draw overlaps
      for (auto& _tuning : session()->tunings())
      {
        if (_tuning.get() == tuning()) continue;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMultMatrixf(tuning()->projector().projectionMatrix().constData());
      glMatrixMode(GL_MODELVIEW);

      glLoadIdentity();

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
          _.glDisable(GL_LIGHTING);
          _.glDisable(GL_CULL_FACE);
          _.glDisable(GL_DEPTH_TEST);
          session_->drawFrustumIntersection(_tuning->projector(),_tuning->color(),ProjectorViewMode::BOTH);
          _.glDisable(GL_DEPTH_TEST);
      });

      }
      */

      if (showCursor_ || !viewOnly_)
         vizTuning_->drawCursor(cursorPosition_);
    }

    void TuningGLView::drawOutput(float _blendMaskOpacity, float _inputOpacity, QColor _color) {

      updateWarpBuffer();

      drawOnSurface([&](QOpenGLFunctions& _)
      {
        _.glDisable(GL_LIGHTING);
        _.glEnable(GL_BLEND);
        vizTuning_->drawOutput(
            warpGridBuffer_->texture(),
            _inputOpacity,_color,_blendMaskOpacity,
            tuning()->outputDisabled() && viewOnly());
        drawScreenBorder();
      });
    }

    void TuningGLView::drawColorCorrected()
    {
      drawOutput(
          1.0 /* draw blend mask with alpha = 1.0 */,
          1.0 /* draw input */ );
    }

    void TuningGLView::drawExportView()
    {
      drawOutput(
          1.0 /* draw blend mask with alpha = 1.0 */,
          1.0 /* draw input */ );
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
        vizTuning_->drawTestCard(index()+1,tuning()->outputDisabled() && viewOnly());
      });
    }

    void TuningGLView::paintGL()
    {
      if (!isVisible() || this->isLocked( )|| !context() || aboutToBeDestroyed_) return;

      if (!vizTuning_ || !session_ || !tuning()) return;

      if (tuning()->outputDisabled() && this->isFullScreen()) return;

      session_->update();

      if (!vizTuning_->initialized())
        vizTuning_->update();

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
      case Session::Mode::ARRANGE:
        drawCanvas();
        break;
      case Session::Mode::WARP:
        drawWarpGrid();
        break;
      case Session::Mode::BLEND:
        drawBlendMask();
        break;
      case Session::Mode::COLORCORRECTION:
        drawColorCorrected();
        break;
      case Session::Mode::EXPORT:
        drawExportView();
        break;
      case Session::Mode::LIVE:
        //drawLiveView();
        break;
      default: break;
      }
      glPopAttrib();
    }
  }
}

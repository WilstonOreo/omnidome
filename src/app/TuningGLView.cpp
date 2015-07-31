#include <omni/ui/TuningGLView.h>

#include <QDebug>
#include <QResizeEvent>
#include <QMatrix4x4>

namespace omni
{
  namespace ui
  {
    TuningGLView::TuningGLView(QWidget* _parent) :
      GLView(_parent)
    {
    }

    TuningGLView::~TuningGLView()
    {
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
    }

    bool TuningGLView::keepAspectRatio() const
    {
      return keepAspectRatio_;
    }

    void TuningGLView::setKeepAspectRatio(bool _keepAspectRatio)
    {
      keepAspectRatio_ = _keepAspectRatio;
      sizePolicy().setHeightForWidth(keepAspectRatio_);
      update();
    }

    bool TuningGLView::viewOnly() const
    {
      return viewOnly_;
    }

    void TuningGLView::setViewOnly(bool _viewOnly)
    {
      viewOnly_ = _viewOnly;
      update();
    }
      
    bool TuningGLView::isDrawingEnabled() const
    {
      return drawingEnabled_;
    }

    void TuningGLView::setDrawingEnabled(bool _drawingEnabled) 
    {
      drawingEnabled_ = _drawingEnabled;
      update();
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
      setParent(nullptr);
      setWindowFlags( Qt::CustomizeWindowHint );
      setWindowFlags(Qt::FramelessWindowHint);
      raise();
      setGeometry(_screen.rect());
      fullscreen_ = true;
    }

    void TuningGLView::mouseMoveEvent(QMouseEvent *event)
    {
    }

    void TuningGLView::mousePressEvent(QMouseEvent *event)
    {
      GLView::mousePressEvent(event);
    }

    void TuningGLView::mouseReleaseEvent(QMouseEvent *event)
    {
    }

    void TuningGLView::wheelEvent(QWheelEvent* event)
    {
    }

    void TuningGLView::keyPressEvent(QKeyEvent* event)
    {
    }
      
    bool TuningGLView::initialize()
    {
      if (context())
        frameBuffer_.reset(new QOpenGLFramebufferObject(512,512));
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

    void TuningGLView::paintGL()
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (!tuning_) return;

      if (!isDrawingEnabled())
        if (frameBuffer_)
        {
          frameBuffer_->bind();
          glViewport(0, 0, (GLint)frameBuffer_->width(), (GLint)frameBuffer_->height());
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
      
      glDisable(GL_BLEND);

      switch (session()->mode()) 
      {
      case Session::Mode::SCREENSETUP:
        //tuning_.drawScreenImage();
        break;
      case Session::Mode::PROJECTIONSETUP: 
        drawCanvas();
        break;
      case Session::Mode::WARP:
      case Session::Mode::BLEND:
      case Session::Mode::EXPORT:
        break;
      }
      
      if (!isDrawingEnabled())
      {
        if (frameBuffer_)
        {
          frameBuffer_->release();
          makeCurrent();
          
          glViewport(0,0, width(), height());
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

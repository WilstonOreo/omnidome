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
      return true;
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

    void TuningGLView::paintGL()
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (!tuning_) return;

      switch (session()->mode()) 
      {
      case Session::Mode::SCREENSETUP:
        //tuning_.drawScreenImage();
        break;
      case Session::Mode::PROJECTIONSETUP: 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMultMatrixf(tuning()->projector().projectionMatrix().constData());
        glMatrixMode(GL_MODELVIEW);
     
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        session_->drawCanvas();
        glPopMatrix();
        break;
      case Session::Mode::WARP:
      case Session::Mode::BLEND:
      case Session::Mode::EXPORT:
        break;
      }
    }
  }
}

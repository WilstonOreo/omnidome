#include <omni/visual/Surface.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    bool Surface::ignoreAspect() const
    {
      return ignoreAspect_;
    }
    void Surface::setIgnoreAspect(bool _ignoreAspect)
    {
      ignoreAspect_ = _ignoreAspect;
    }
      
    bool Surface::upsideDown() const
    {
      return upsideDown_;
    }

    void Surface::setUpsideDown(bool _upsideDown)
    {
      upsideDown_=_upsideDown;
    }
  
    qreal Surface::border() const
    {
      return border_;
    }

    void Surface::setBorder(qreal _border)
    {
      border_=_border;
    }
 
    QSize Surface::sceneSize() const
    {
      return sceneSize_;
    }

    void Surface::setSceneSize(QSize _sceneSize) 
    {
      sceneSize_=_sceneSize; 
    }

    QRectF Surface::sceneRect(bool _upsideDown) const
    {
      qreal _sceneAspect = util::aspect(sceneSize_);
      qreal _windowAspect = util::aspect(windowSize_);
      float b = border_ * 0.5;
      float _left = -0.5 - b,_right = 0.5 + b,_bottom = -0.5 - b,_top = 0.5 + b;

      if (!upsideDown_)
      {
        std::swap(_bottom,_top);
      }

      if (!ignoreAspect())
      {
        if (_windowAspect > _sceneAspect)
        {
          _top *= _windowAspect / _sceneAspect;
          _bottom *=  _windowAspect / _sceneAspect;
        }
        else
        {
          _left *= _sceneAspect / _windowAspect;
          _right *= _sceneAspect / _windowAspect;
        }
      }
      return QRectF(QPointF(_left,_top),QPointF(_right,_bottom));
    }

    QSize Surface::windowSize() const
    {
      return windowSize_;
    }

    void Surface::setWindowSize(QSize _windowSize)
    {
      windowSize_ = _windowSize;
    }
  }
}

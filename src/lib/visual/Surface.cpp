#include <omni/visual/Surface.h>

namespace omni
{
  namespace visual
  {
    QRectF Surface::sceneRect(bool _upsideDown) const;
    {
      qreal _sceneAspect = Scalar(mapping()->width()) / mapping()->height();
      qreal _windowAspect = Scalar(width()) / height();
      float b = border_ * 0.5;
      float _left = -0.5 - b,_right = 0.5 + b,_bottom = -0.5 - b,_top = 0.5 + b;

      if (!upsideDown_)
      {
        std::swap(_bottom,_top);
      }

      if (!ignoreAspect())
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
  }
}

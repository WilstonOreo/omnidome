#include <omni/BlendMask.h>

#include <omni/proj/Tuning.h>

namespace omni
{
  BlendMask::BlendMask(proj::Tuning const& _tuning) :
    tuning_(_tuning),
    rect_(0.1,0.1,0.8,0.9),
    gamma_(2.0)
  {
    update();
  }

  void BlendMask::clear()
  {
    strokeBuffer_.clear();
    update();
  }

  void BlendMask::update()
  {  
    int w = tuning_.width();
    int h = tuning_.height();
    maskBuffer_.resize(w,h);

    for (int y = 0; y < maskBuffer_.height(); ++y)
      for (int x = 0; x < maskBuffer_.width(); ++x)
      {
        maskBuffer_(x,y) = 255.0 * qBound(borderValue(float(x) / w,float(y) / h),0.0f,1.0f);
      }
  }

  void BlendMask::setRect(QRectF const& _rect, bool _update)
  {
    rect_ = _rect;
    if (_update) update();
  }

  QRectF BlendMask::rect() const
  {
    return rect_;
  }

  float BlendMask::topWidth() const
  {
    return rect_.top();
  }

  float BlendMask::bottomWidth() const
  {
    return 1.0 - rect_.bottom();
  }

  float BlendMask::leftWidth() const
  {
    return rect_.left();
  }

  float BlendMask::rightWidth() const
  {
    return 1.0 - rect_.right();
  }

  void BlendMask::setGamma(float _gamma)
  {
    gamma_=_gamma;
    if (gamma_ < 0.1) gamma_ = 0.1;
  }

  float BlendMask::gamma() const
  {
    return gamma_;
  }
    
  BlendBrush& BlendMask::brush()
  {
    return brush_;
  }

  BlendBrush const& BlendMask::brush() const
  {
    return brush_;
  }

  void BlendMask::stamp(const QPointF& _pos)
  {
    brush_.stamp(_pos,strokeBuffer_);
  }

  float BlendMask::drawLine(QPointF const& _p0, QPointF const& _p1, float _leftOver)
  {
    return brush_.drawLine(_p0,_p1,strokeBuffer_,_leftOver);
  }
    
  BlendBuffer const& BlendMask::strokeBuffer() const
  {
    return strokeBuffer_;
  }

  BlendBuffer const& BlendMask::maskBuffer() const
  {
    return maskBuffer_;
  }

  float BlendMask::borderValue(float _x, float _y) const
  {
    float edgeValue = 1.0;
    if (_x <= leftWidth())
    {
      edgeValue *= std::min(_x / leftWidth(), 1.0f);
    }
    else if (_x >= 1.0f - rightWidth())
    {
      edgeValue *= std::min((1.0f - _x) / rightWidth(), 1.0f);
    }
    if (_y <= topWidth())
    {
      edgeValue *= std::min(_y / topWidth(), 1.0f);
    }
    if (_y >= 1.0f - bottomWidth())
    {
      edgeValue *= std::min((1.0f - _y) / bottomWidth(), 1.0f);
    }
    return 1.0f - pow(qBound(edgeValue,0.0f,1.0f),gamma_);
  }
}

QDataStream& operator>>(QDataStream& _is, omni::BlendMask& _mask)
{
  QRectF _rect;
  _is >> _rect;
  float _gamma;
  _is >> _gamma;
  _is >> _mask.brush();

  _mask.setGamma(_gamma);
  _mask.setRect(_rect);
  return _is;
}

QDataStream& operator<<(QDataStream& _os, omni::BlendMask const& _mask)
{
  _os << _mask.rect();
  _os << _mask.gamma();
  _os << _mask.brush();
  return _os;
}


#include <omni/BlendBrush.h>

#include <QPointF>
#include <QVector2D>
#include <QDataStream>

namespace omni
{
  BlendBrush::BlendBrush()
  {
    generate();
  }

  BlendBrush::BlendBrush(float _size, float _feather) :
    size_(_size),
    feather_(_feather)
  {
    generate();
  }



  float BlendBrush::size() const
  {
    return size_;
  }

  void BlendBrush::setSize(float _size)
  {
    size_= qBound(_size,2.0f,512.0f);
    generate();
  }

  float BlendBrush::feather() const
  {
    return feather_;
  }

  void BlendBrush::setFeather(float _feather)
  {
    feather_=_feather;
    if (feather_ < 0.0) feather_ = 0.0;
    if (feather_ > 1.0) feather_ = 1.0;
    generate();
  }

  bool BlendBrush::invert() const
  {
    return invert_;
  }

  void BlendBrush::setInvert(bool _invert)
  {
    invert_=_invert;
    generate();
  }

  void BlendBrush::setBrush(float _size, float _feather, bool _invert)
  {
    size_= qBound(_size,2.0f,512.0f);
    feather_=_feather;
    if (feather_ < 0.0) feather_ = 0.0;
    if (feather_ > 1.0) feather_ = 1.0;
    invert_=_invert;
    generate();
  }

  void BlendBrush::stamp(QPointF const& _p, BlendBuffer& _buf) const
  {
    int _size = size();
    float r = size()*0.5;

    for (int i = 0; i < _size; ++i)
    {
      int _posx = int(i + _p.x() - r*0.5);
      if ((_posx < 0) || (_posx >= _buf.width())) continue;
      for (int j = 0; j < _size; ++j)
      {
        int _posy = int(j + _p.y()-r*0.5);

        if ((_posy < 0) || (_posy >= _buf.height())) continue;

        float _v = buffer_(i,j) / 255.0;
        auto& _pix = _buf(_posx,_posy);

        if (invert())
        {
          _pix *= _v;
        }
        else
        {
          _pix = 255.0 - 255.0 * _v + _buf(_posx,_posy) * _v;
        }
      }
    }
  }

  float BlendBrush::drawLine(const QPointF& _p0, const QPointF& _p1,
                             BlendBuffer& _buf, float _leftOver)
  {
    float _spacing = size_ / 20.0;
    if (_spacing < 0.5) _spacing = 0.5;

    QVector2D _step(0.0,0.0);
    
    // Calculate vector and distance
    QVector2D _delta(_p1 - _p0);
    float _dist = _delta.length();

    float _invDist = 0.0;
    if (_dist > 0.0)
    {
      _invDist = 1.0 / _dist;
      _step += _delta * _invDist;
    }

    // Draw line
    QVector2D _offset(0.0,0.0);
    float _totalDistance = _leftOver + _dist;
    while (_totalDistance >= _spacing)
    {
      if (_leftOver > 0)
      {
        _offset += _step * (_spacing - _leftOver);
        _leftOver -= _spacing;
      }
      else
      {
        _offset += _step * _spacing;
      }
      // Draw stamp
      stamp(_p0 + _offset.toPointF(),_buf);
      
      _totalDistance -= _spacing;
    }

    return _totalDistance;
  }


  void BlendBrush::generate()
  {
    auto _size = size();
    buffer_.resize(_size,_size);

    float _r = _size*0.5;
    // Calculate feather radius
    float _innerRadius = feather() * (1.0 - _r) + _r;

    // For each pixel
    for (int y = 0; y < _size; ++y)
      for (int x = 0; x < _size; ++x)
      {
        float _distance = QVector2D(x - _r,y - _r).length();

        // Pixel value
        float _v = 256.0 * (_distance - _innerRadius) / (_r - _innerRadius);

        // Clamp and set pixel value
        buffer_(x,y) = qBound(_v,0.0f,255.0f);
      }
  }

}

QDataStream& operator>>(QDataStream& _is, omni::BlendBrush& _brush)
{
  float _size;
  _is >> _size;
  float _feather;
  _is >> _feather;
  bool _invert;
  _is >> _invert;
  _brush.setBrush(_size,_feather,_invert);
  return _is;
}

QDataStream& operator<<(QDataStream& _os, omni::BlendBrush const& _brush)
{
  _os << _brush.size() << _brush.feather() << _brush.invert();
  return _os;
}


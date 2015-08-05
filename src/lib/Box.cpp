#include <omni/Box.h>

#include <limits>

namespace omni
{
  Box::Box()  
  {
    auto _min = std::numeric_limits<float>::min();
    auto _max = std::numeric_limits<float>::max();
    min_ = QVector3D(_max,_max,_max);
    max_ = QVector3D(_min,_min,_min);
  }

  Box::Box(QVector3D const& _min, QVector3D const& _max) 
  {
    setMinMax(_min,_max);
  }

  QVector3D Box::size() const
  {
    return max() - min();
  }

  qreal Box::radius() const
  {
    return size().length() * 0.5; 
  }

  QVector3D Box::min() const
  {
    return min_;
  }

  QVector3D Box::max() const
  {
    return max_;
  }
  
  void Box::setMin(QVector3D const& _min)
  {
    min_ = _min;
    validate();
  }

  void Box::setMax(QVector3D const& _max)
  {
    max_ = _max;
    validate();
  }

  void Box::setMinMax(QVector3D const& _min, QVector3D const& _max)
  {
    min_ = _min;
    max_ = _max;
    validate();
  }

  void Box::validate()
  {
    for (int i = 0; i < 3; ++i)
    {
      if (min_[i] > max_[i]) std::swap(min_[i],max_[i]);
    }
  }
}

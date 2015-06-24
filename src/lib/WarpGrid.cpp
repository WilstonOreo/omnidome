#include <omni/WarpGrid.h>

#include <omni/util.h>
#include <QDataStream>

namespace omni
{
  WarpGrid::WarpGrid()
  {
    resize(6,6);
  }

  QVector2D WarpGrid::getTexCoord(int _x, int _y) const
  {
    return QVector2D(float(_x)/(horizontal()-1),float(_y)/(vertical()-1));
  }

  void WarpGrid::reset()
  {
    points_.clear();
    points_.reserve(vertical()*horizontal());
    for (size_t y = 0; y < vertical(); ++y)
      for (size_t x = 0; x < horizontal(); ++x)
        points_.emplace_back((getTexCoord(x,y)-QVector2D(0.5,0.5)).toPointF());
  }

  void WarpGrid::resize(int _horz, int _vert)
  {
    if (_horz < 2 || _vert < 2) return;
    horizontal_ = _horz;
    vertical_ = _vert;
    reset();
  }

  int WarpGrid::vertical() const
  {
    return vertical_;
  }
  
  int WarpGrid::horizontal() const
  {
    return horizontal_;
  }

  void WarpGrid::selectAll()
  {
    for (auto& _point : points_)
      _point.setSelected(true);
  }

  WarpPoint* WarpGrid::selectNearest(const QPointF& _p)
  {
    size_t _nearestIdx = getNearest(_p);
    auto _nearest = (_nearestIdx == -1) ? nullptr : &points_[_nearestIdx];
    return _nearest;
  }

  void WarpGrid::selectNone()
  {
    for (auto& _point : points_)
      _point.setSelected(false);
  }

  std::set<WarpPoint*> WarpGrid::getSelected()
  {
    std::set<WarpPoint*> _selection;
    for (auto& _point : points_)
      if (_point.selected())
        _selection.insert(&_point);
    return _selection;
  }

  std::set<WarpPoint const*> WarpGrid::getSelected() const
  {
    std::set<WarpPoint const*> _selection;
    for (auto& _point : points_)
      if (_point.selected())
        _selection.insert(&_point);
    return _selection;
  }

  WarpPoint* WarpGrid::getPoint(int x, int y)
  {
    if (x < 0 || y < 0) return nullptr;
    if (x >= horizontal_ || y >= vertical_) return nullptr;
    return &points_[y*horizontal_+x];
  }

  WarpPoint const* WarpGrid::getPoint(int x, int y) const
  {
    if (x < 0 || y < 0) return nullptr;
    if (x >= horizontal_ || y >= vertical_) return nullptr;
    return &points_[y*horizontal_+x];
  }

  size_t WarpGrid::getNearest(const QPointF& _p) const
  {
    float _nearestDist = std::numeric_limits<float>::max();
    size_t _nearestIdx = -1;
    for (size_t i = 0; i < points_.size(); ++i)
    {
      auto& _point = points_[i];
      auto _dist = QVector2D(_p - _point.pos()).lengthSquared();
      if (_dist < _nearestDist)
      {
        _nearestIdx = i;
        _nearestDist = _dist;
      }
    }
    return _nearestIdx;
  }

  bool WarpGrid::isReset() const
  {
    for (size_t y = 0; y < vertical(); ++y)
      for (size_t x = 0; x < horizontal(); ++x)
      {
        if (QVector2D(getPoint(x,y)->pos()) != (getTexCoord(x,y)-QVector2D(0.5,0.5)))
          return false;
      }

    return true;
  }
 
  std::vector<WarpPoint> const& WarpGrid::points() const
  {
    return points_;
  }
  
  bool operator==(WarpGrid const& _lhs,WarpGrid const& _rhs)
  {
    return 
      OMNI_TEST_MEMBER_EQUAL(horizontal_) &&
      OMNI_TEST_MEMBER_EQUAL(vertical_) &&
      OMNI_TEST_MEMBER_EQUAL(points_);
  }
}

QDataStream& operator<<(QDataStream& _os, const omni::WarpGrid& _p)
{
  _os << _p.horizontal() << _p.vertical();
  for (auto& _point : _p.points())
    _os << _point;
  return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::WarpGrid& _p)
{
  int _horizontal, _vertical;
  _is >> _horizontal >> _vertical;
  _p.resize(_horizontal,_vertical);
  for (int y = 0; y < _p.vertical(); ++y)
    for (int x = 0; x < _p.horizontal(); ++x)  
      _is >> *_p.getPoint(x,y);

  return _is;
}

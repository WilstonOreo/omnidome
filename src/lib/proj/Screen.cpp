#include <omni/proj/Screen.h>

namespace omni
{
  namespace proj
  {
    Screen::Screen(QRect const& _rect, int _index) :
      rect_(_rect),
      index_(_index)
    {}

    bool Screen::hasSubScreens() const
    {
      return subScreenCount() > 1;
    }

    int Screen::width() const
    {
      return rect_.width() / subScreenCount();
    }

    int Screen::height() const
    {
      return rect_.height();
    }
      
    qreal Screen::aspectRatio() const
    {
      return width() / qreal(height());
    }

    int Screen::subScreenCount() const
    {
      return subScreenCount(rect_);
    }
      
    int Screen::subScreenCount(QRect const& _rect)
    {
      auto _s = _rect.size();
      qreal _aspectRatio = _s.width() / qreal(_s.height());

      if (_aspectRatio < 1.5) return 1;

      // Detect triple head
      size_t _subscreenCount = 3;
      while (_rect.width() % _subscreenCount == 0)
      {
        --_subscreenCount;
        if (_subscreenCount == 1) break;
      }
      return _subscreenCount;
    }

    QRect Screen::rect() const
    {
      return rect_;
    }
  }
}

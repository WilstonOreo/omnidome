#include <omni/proj/ScreenSetup.h>

#include <QApplication>
#include <QDesktopWidget>

namespace omni
{
  namespace proj
  {
    ScreenSetup::ScreenSetup()
    {
    }
  
    QScreen const* ScreenSetup::standardScreen()
    {
      return QGuiApplication::primaryScreen();
    }
 
    int ScreenSetup::subScreenCount(QScreen const* _screen)
    {
      auto _rect = _screen->geometry();
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
      if (_subscreenCount <= 0) 
        _subscreenCount = 1;
      return _subscreenCount;
    }

    int ScreenSetup::subScreenWidth(QScreen const* _screen)
    {
      return _screen->size().width() / ScreenSetup::subScreenCount(_screen);
    }
 
    QRect ScreenSetup::subScreenRect(QScreen const* _screen, int _subScreenIndex)
    {
      int _w = ScreenSetup::subScreenWidth(_screen);
      return QRect(_w*_subScreenIndex,0,_w,_screen->size().height());
    }
      
    QRect ScreenSetup::desktopRect(bool _excludeStandardScreen)
    {
      QRect _desktopRect;
      auto _screens = ScreenSetup::screens(_excludeStandardScreen);
      for (auto& _screen : _screens)
        _desktopRect |= _screen->geometry();
      return _desktopRect;
    }

    std::vector<QScreen const*> ScreenSetup::screens(bool _excludeStandardScreen)
    {
      std::vector<QScreen const*> _screens;
      auto _allScreens = QGuiApplication::screens();
      for (auto& _screen : _allScreens)
        if (!_excludeStandardScreen || (_screen != standardScreen()))
          _screens.push_back(_screen);
      return _screens;
    }

    bool ScreenSetup::operator==(const ScreenSetup& _rhs) const
    {
      return false;
    }
  }
}

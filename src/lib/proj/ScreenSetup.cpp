#include <omni/proj/ScreenSetup.h>

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

namespace omni
{
  namespace proj
  {
    ScreenSetup::ScreenSetup()
    {
    }

    std::vector<QSize> const& ScreenSetup::screenResolutions()
    {
      static std::vector<QSize> _sizes;
      if (_sizes.empty())
        _sizes = {
        QSize(640,480),
        QSize(800,600),
        QSize(1024,768),
        QSize(1280,720),
        QSize(1280,800),
        QSize(1920,1080),
        QSize(1920,1200)
        };

      return _sizes;
    }

    QScreen const* ScreenSetup::standardScreen()
    {
      return QGuiApplication::primaryScreen();
    }
 
    int ScreenSetup::subScreenCount(QScreen const* _screen)
    {
      // Go through list of screen resolutions and see if the
      // current screen has subscreens
      for (auto& _screenSize :screenResolutions())
      {
        if (_screenSize.height() == _screen->geometry().height())
        {
          if ((_screen->geometry().width() % _screenSize.width()) == 0)
          {
            return _screen->geometry().width() / _screenSize.width();
          }
        }
      }
      return 1;
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

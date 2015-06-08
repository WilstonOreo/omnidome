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

    ScreenSetup ScreenSetup::current()
    {
      ScreenSetup _setup;
      QDesktopWidget* _desktop = QApplication::desktop();
      int _numberScreens = _desktop->screenCount();

      int _index = 0;
      for (int i = 0; i < _numberScreens; ++i)
      {
        QRect _screenRect = _desktop->screenGeometry(i);

        // Exclude main screen
        //if (_screenRect == _desktop->screenGeometry(this)) continue;

        int _subScreens = Screen::subScreenCount(_screenRect);
        for (int i = 0; i < _subScreens; ++i)
        {
          _setup.screens_.emplace_back(_screenRect,_index);
        }
      }

      return _setup;
    }

    ScreenSetup ScreenSetup::debug()
    {
      ScreenSetup _debug;

      QRect _rect(0,0,1920,1080);

      for (int i = 0; i < 3; ++i)
      {
        _rect.moveTo(1920*i,0);
        _debug.screens_.emplace_back(_rect);
      }

      return _debug;

    }
      
    Screen ScreenSetup::standardScreen()
    {
      QDesktopWidget* _desktop = QApplication::desktop(); 
      QRect _screenRect = _desktop->screenGeometry(0);
      return Screen(_screenRect,0);
    }

    ScreenSetup::screenlist_type const& ScreenSetup::screens() const
    {
      return screens_;
    }

    bool ScreenSetup::operator==(const ScreenSetup& _rhs) const
    {
      return false;
    }
  }
}

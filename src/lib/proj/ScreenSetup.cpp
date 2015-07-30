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
    
    ScreenSetup& ScreenSetup::current()
    {
      static ScreenSetup _setup;
      if (QApplication::startingUp()) 
      {
        _setup.screens_.emplace_back(standardScreen());
        return _setup;
      }

      QDesktopWidget* _desktop = QApplication::desktop();
      int _numberScreens = _desktop->screenCount();

      int _index = 0;
      for (int i = 0; i < _numberScreens; ++i)
      {
        QRect _screenRect = _desktop->screenGeometry(i);

        // Exclude main screen
        if (_screenRect == _desktop->screenGeometry()) continue;

        int _subScreens = Screen::subScreenCount(_screenRect);
        _screenRect.setWidth(_screenRect.width() / _subScreens);

        for (int i = 0; i < _subScreens; ++i)
        {
          _setup.screens_.emplace_back(_screenRect,_index);
          _screenRect.translate(_screenRect.width(),0);
        }
      }

      QRect _rect(0,0,320,240);

      for (int i = 0; i < 3; ++i)
      {
        _rect.moveTo(400*i,0);
        _setup.screens_.emplace_back(_rect);
      }

      return _setup;
    }

    ScreenSetup& ScreenSetup::debug()
    {
      static ScreenSetup _debug;


      return _debug;

    }
      
    Screen ScreenSetup::standardScreen()
    {
      // Return a default screen with 1024x768 resolution if there is no QApplication instance 
      if (QCoreApplication::startingUp()) return Screen(QRect(0,0,1024,768));
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

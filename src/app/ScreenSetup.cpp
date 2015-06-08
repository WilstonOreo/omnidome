#include <omni/ui/ScreenSetup.h>

#include <omni/proj/ScreenSetup.h>

#include <QGraphicsScene>
#include <QDebug>

namespace omni
{
  namespace ui
  { 
    ScreenSetup::ScreenSetup(QWidget* _parent) :
      QWidget(_parent)
    {
      updateScreens();
    }

    ScreenSetup::~ScreenSetup()
    {
    }
    
    void ScreenSetup::updateScreens()
    {
      screenItems_.clear();
      desktopRect_ = QRect(0,0,0,0);

      proj::ScreenSetup _setup = proj::ScreenSetup::debug();

      for (auto& _screen : _setup.screens())
      {
        qDebug() << _screen.rect();
        screenItems_.push_back(Item(*this,_screen));

        desktopRect_ |= _screen.rect();
      }
    }

    ScreenSetup::Item::Item(
        ScreenSetup& _screenSetup,
        Screen const& _screen) :
      screenSetup_(_screenSetup),
      screen_(_screen)
    {
    
    }
    
    void ScreenSetup::paintEvent(QPaintEvent*)
    {
    }

    void ScreenSetup::Item::paint(QPainter* _painter)
    {
    }

    void ScreenSetup::Item::attachTuning(omni::proj::Tuning* _tuning)
    {
      tuning_=_tuning;
      screenSetup_.update();
    }

    void ScreenSetup::Item::detachTuning()
    {
      tuning_ = nullptr;
      screenSetup_.update();
    }
  }
}

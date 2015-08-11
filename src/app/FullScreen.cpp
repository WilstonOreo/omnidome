#include <omni/ui/FullScreen.h>

#include <omni/ui/ScreenSetup.h>
#include <omni/proj/ScreenSetup.h>

namespace omni
{
  namespace ui
  {
    FullScreen::FullScreen(const QScreen* _screen) :
      screen_(_screen),
      dragWidget_(new ScreenSetupDragWidget(this))
    {
      setStyleSheet("* { background:black; }");
      setWindowFlags( Qt::CustomizeWindowHint );
      setWindowFlags(Qt::FramelessWindowHint);
      
      move(_screen->geometry().topLeft());
      this->hide();
    }

    FullScreen::~FullScreen()
    {
      detachAll();
    }

    void FullScreen::detachAll()
    {
      for (auto& _widget : widgets_) 
      {
        _widget.second->hide();
        _widget.second->setParent(nullptr);
      }
      widgets_.clear();
      hide();
    }

    void FullScreen::detach(int _subIndex)
    {
      if (widgets_.count(_subIndex) == 0) return;

      widgets_[_subIndex]->hide();
      widgets_[_subIndex]->setParent(nullptr);
      widgets_.erase(_subIndex);
      if (widgets_.empty())
        hide();
    }

    void FullScreen::attach(int _subIndex, QWidget* _widget)
    {
      if (widgets_.count(_subIndex) > 0)
      {
        detach(_subIndex);
      }

      widgets_[_subIndex] = _widget;

      setGeometry(_widget,_subIndex);
      _widget->setParent(this);
      _widget->show();
      showFullScreen();
    }
 
    void FullScreen::hideDragWidget()
    {
      dragWidget_->hide();
      if (widgets_.empty())
        hide();
    }
 
    void FullScreen::setGeometry(QWidget* _widget, int _subIndex) const
    {
      int _subScreenCount = omni::proj::ScreenSetup::subScreenCount(screen_);
 
      if (_subIndex <  0) _subIndex = 0;
      if (_subIndex >= _subScreenCount) _subIndex = _subScreenCount - 1;

      _widget->setGeometry(omni::proj::ScreenSetup::subScreenRect(screen_,_subIndex));
    }
 
    void FullScreen::showDragWidget(int _subIndex, QColor const& _color)
    {
      setGeometry(dragWidget_.get(),_subIndex);
      dragWidget_->setColor(_color);
      dragWidget_->show();
      dragWidget_->raise();
      showFullScreen();
    }

    void FullScreen::closeEvent(QCloseEvent* _event)
    {
      detachAll();
    }
  }
}

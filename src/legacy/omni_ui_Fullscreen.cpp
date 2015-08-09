#include <omni/ui/Fullscreen.h>

#include <omni/debug.hpp>

using namespace omni::ui;

Fullscreen::Fullscreen(const proj::Screen& _screen) :
  screen_(_screen)
{
  setWindowFlags( Qt::CustomizeWindowHint );
  setWindowFlags(Qt::FramelessWindowHint);
  move(_screen.x(),_screen.y());
  this->hide();
  transparent(false);

  size_t _subScreenCount = screen().subScreenCount();
  for (size_t _subIndex = 0; _subIndex < _subScreenCount; ++_subIndex)
  {
    size_t _w = screen_.width() / _subScreenCount;
    identifiers_.emplace_back(new ScreenIdentifier(screen().index(),_subIndex));
    identifiers_.back()->setGeometry(_w*_subIndex,0,_w,screen().height());
    identifiers_.back()->setParent(this);
  }
}

Fullscreen::~Fullscreen()
{
  /* FIXME: Crashes when screens are changing
   * Possible solution: Handle QScreenChangedEvent properly
   * for (auto& _idWidget : widgets_)
        {
          detach(_idWidget.first);
        }*/
}

void Fullscreen::transparent(bool _transparent)
{
  transparent_=_transparent;
  if (_transparent)
  {
    setAttribute(Qt::WA_TranslucentBackground,_transparent);
    setStyleSheet("* {background:transparent; }");
  }
  else
  {
    setStyleSheet("* { background:black; }");
  }
}

void Fullscreen::clear()
{
  for (size_t _subIndex = 0; _subIndex < screen().subScreenCount(); ++_subIndex)
  {
    detach(_subIndex);
  }
}

void Fullscreen::identify(bool _identify)
{
  OMNI_DBG("Fullscreen" << screen().index())
  showFullScreen();
  raise();
  for (auto& _identifier : identifiers_)
  {
    _identifier->setVisible(_identify);
  }
  for (auto& _idWidget : widgets_)
  {
    auto _widget = _idWidget.second;
    if (!_widget) continue;
    _widget->setVisible(!_identify);
  }
}

void Fullscreen::detach(size_t _subIndex)
{
  if (widgets_.count(_subIndex) == 0) return;

  widgets_[_subIndex]->hide();
  widgets_[_subIndex]->setParent(nullptr);
  widgets_.erase(_subIndex);
}

void Fullscreen::place(size_t _subIndex, QWidget* _widget)
{
  if (widgets_.count(_subIndex) > 0)
  {
    detach(_subIndex);
  }

  widgets_[_subIndex] = _widget;
  size_t _subScreenCount = screen_.subScreenCount();
  size_t _w = screen_.width() / _subScreenCount;
  _widget->setParent(this);
  _widget->setGeometry(_w*_subIndex,0,_w,screen_.height());
  _widget->show();
}

void Fullscreen::closeEvent(QCloseEvent* _event)
{
  for (auto& _idWidget : widgets_)
  {
    detach(_idWidget.first);
  }
}

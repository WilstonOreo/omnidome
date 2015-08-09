#include <omni/ui/ScreenSetup.h>

#include <omni/Session.h>
#include <omni/ui/proj/Tuning.h>
#include <omni/proj/ScreenSetup.h>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

namespace omni
{
  namespace ui
  { 
    ScreenSetup::ScreenSetup(QWidget* _parent) :
      QWidget(_parent),
      dragWidget_(new ScreenSetupDragWidget)
    {
      setMouseTracking(true);
      setAcceptDrops(true);
      updateScreens();
    }

    ScreenSetup::~ScreenSetup()
    {
    }
      
    Session const* ScreenSetup::session() const
    {
      return session_;
    }
      
    void ScreenSetup::setSession(Session* _session)
    {
      session_ = _session;
      updateScreens();
    }
      
    float ScreenSetup::zoom() const
    {
      return zoom_;
    }

    void ScreenSetup::setZoom(float _zoom) 
    {
      zoom_=_zoom;
      update();
    }
 
    void ScreenSetup::detachTuning(omni::ui::proj::Tuning* _tuning)
    {
      if (!_tuning) return;

      for (auto& _item : screenItems_)
        if (_item.tuning() == _tuning)
        {
          qDebug() << _tuning->index();
          _item.detachTuning();
        }
      update();
    }
 
    void ScreenSetup::updateScreens()
    {
      screenItems_.clear();
      desktopRect_ = QRect(0,0,0,0);

      setup_ = session_ ? &session_->screenSetup() : &omni::proj::ScreenSetup::current();

      for (auto& _screen : setup_->screens())
      {
        screenItems_.push_back(Item(*this,_screen));
        desktopRect_ |= _screen.rect();
      }
    }
 
    float ScreenSetup::scalingFactor() const 
    {
      auto _windowRect = this->rect();
      float _rectAspect = desktopRect_.width() / desktopRect_.height();
      float _windowAspect = _windowRect.width() / _windowRect.height();

      float _factor = 1.0f;

      if (_rectAspect > _windowAspect) {                
        _factor = _windowRect.width() / desktopRect_.width();
      } else {
        _factor = _windowRect.height() / desktopRect_.height();
      }

      return _factor * zoom();
    }

    QRectF ScreenSetup::transformedRect() const
    {
      auto _windowRect = this->rect();
      auto _zoom = scalingFactor();
      return QRectF(
          0.5*(_windowRect.width() - _zoom * (desktopRect_.width())),
          0.5*(_windowRect.height() - _zoom * (desktopRect_.height())),
          _zoom * desktopRect_.width(),
          _zoom * desktopRect_.height());
    }

    QRectF ScreenSetup::transformedRect(QRectF const& _rect) const
    {
      auto _zoom = scalingFactor();

      /// Transformed desktop rect
      auto _desktopRect = transformedRect();

      return QRectF(
          _desktopRect.x() + _rect.x() * _zoom,
          _desktopRect.y() + _rect.y() * _zoom,
          _rect.width() * _zoom,
          _rect.height() * _zoom);
    }
 
    void ScreenSetup::paintEvent(QPaintEvent*)
    {
      QPainter _p(this);
      
      _p.setPen(QPen(QColor("#ffffff").darker(150),3));
      _p.drawRect(transformedRect());

      /// Draw all items
      for (auto& _item : screenItems_)
      {
        _item.paint(_p);
      }
    }
      
    void ScreenSetup::mouseMoveEvent(QMouseEvent* _event)
    {
      bool _update = false;
      
      for (auto& _item : screenItems_)
      {
        bool _pointInRect = _item.rect().contains(_event->pos());
        _update |= _pointInRect != _item.mouseOver();
        _item.setMouseOver(_pointInRect);
      }

      if (_update)
        update();
    }
      
    ScreenSetup::Item* ScreenSetup::getItemAtPos(QPoint const& _pos)
    {
      Item* _itemAtPos = nullptr;
      for (auto& _item : screenItems_)
      {
        if (_item.rect().contains(_pos)) 
        {
          _itemAtPos = &_item;
          break;
        }
      }

      return _itemAtPos;
    }
 
    void ScreenSetup::dragEnterEvent(QDragEnterEvent* event)
    {
      auto* _screenItem = getItemAtPos(event->pos());
      for (auto& _item : screenItems_)
        _item.setDrop(false);
  
      if (event->mimeData()->hasFormat("text/plain"))
      {
        if (_screenItem) 
        {
          auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
          if (!_tuningWidget->tuning()) return;

          auto _color = _tuningWidget->tuning()->color();
          _screenItem->setDrop(true,_color);
          dragWidget_->setColor(_color);
          dragWidget_->setRect(_screenItem->rect());
          dragWidget_->show();
        }

        event->acceptProposedAction();
      }
      update();
    }

    void ScreenSetup::dragMoveEvent(QDragMoveEvent* event)
    {
      auto* _screenItem = getItemAtPos(event->pos());
      if (!_screenItem) 
        dragWidget_->hide();
      
      for (auto& _item : screenItems_)
        _item.setDrop(false);
  
      if (event->mimeData()->hasFormat("text/plain") && _screenItem)
      {
        dragWidget_->setRect(_screenItem->rect());
        
        auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
        if (!_tuningWidget->tuning()) return;
       
        auto _color = _tuningWidget->tuning()->color();
        _screenItem->setDrop(true,_color);
        dragWidget_->setColor(_color);
        dragWidget_->show();
        event->acceptProposedAction();
      }
      update();
    }

    void ScreenSetup::dropEvent(QDropEvent* event)
    { 
      for (auto& _item : screenItems_)
        _item.setDrop(false);

      auto* _screenItem = getItemAtPos(event->pos()); 
      dragWidget_->hide();

      if (_screenItem) 
      {
        _screenItem->setDrop(false);
        auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
        if (!_tuningWidget->tuning()) return;
        
        _screenItem->attachTuning(_tuningWidget); 
        event->acceptProposedAction();
      }
    }


    /// ScreenSetup::Item 
    /////////////////////////////////////////
    
    ScreenSetup::Item::Item(
        ScreenSetup& _screenSetup,
        Screen const& _screen) :
      screenSetup_(_screenSetup),
      screen_(_screen)
    {
    
    }

    bool ScreenSetup::Item::mouseOver() const
    {
      return mouseOver_;
    }

    void ScreenSetup::Item::setMouseOver(bool _mouseOver) 
    {
      mouseOver_=_mouseOver;
    }

    bool ScreenSetup::Item::drop() const
    {
      return drop_;
    }

    void ScreenSetup::Item::setDrop(bool _drop, QColor const& _color) 
    {
      drop_=_drop;
      dropColor_ = _color;
    }

    QRectF ScreenSetup::Item::rect() const
    {
      return screenSetup_.transformedRect(screen_.rect());
    }

    void ScreenSetup::Item::paint(QPainter& _p)
    {
      // Draw rectangle with tuning color
      QColor _color = tuning_ != nullptr ? tuning_->tuning()->color() : "#cccccc";

      if (mouseOver_)
      {
        _color = _color.lighter(120);
      }

      _p.setBrush(_color);
      _p.setPen(QPen(_color.darker(200),1));

      auto _rect = rect();
      _p.drawRect(_rect); 

      /// Draw diagonal stripes for drop
      if (drop())
      {
        QPixmap _pixmap(32,32);
        _pixmap.fill(_color);
        QPainter _texP(&_pixmap);
        _texP.setPen(QPen(dropColor_,2));
        for (int i = -32; i <= 64; i+= 8)
        {
          _texP.drawLine(i+32,i-32,i-32,i+32);
        }
        _texP.end();
        QBrush _brush;
        _brush.setTexture(_pixmap);
        _p.setBrush(_brush);
        _p.drawRect(_rect); 
      }

      // Draw resolution text
      _p.setBrush(Qt::NoBrush);
      _p.setPen(QPen(_color.darker(500),1));


      // Resolution string
      QString _resStr = QString("%1 x %2").arg(screen_.width()).arg(screen_.height());

      _p.drawText(_rect,Qt::AlignHCenter | Qt::AlignVCenter,_resStr);
    }
        
    omni::ui::proj::Tuning* ScreenSetup::Item::tuning()
    {
      return tuning_;
    }

    omni::ui::proj::Tuning const* ScreenSetup::Item::tuning() const
    {
      return tuning_;
    }

    void ScreenSetup::Item::attachTuning(omni::ui::proj::Tuning* _tuning)
    {
      tuning_=_tuning;
      tuning_->attachScreen(screen_);
      screenSetup_.update();
    }

    void ScreenSetup::Item::detachTuning()
    {
      if (tuning_) 
      {
        tuning_->detachScreen();
      }
      tuning_ = nullptr;
      screenSetup_.update();
    }
  }
}

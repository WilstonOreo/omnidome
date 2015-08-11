#include <omni/ui/ScreenSetup.h>

#include <omni/Session.h>
#include <omni/ui/proj/Tuning.h>
#include <omni/ui/TuningGLView.h>
#include <omni/proj/ScreenSetup.h>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QGuiApplication>

namespace omni
{
  namespace ui
  { 
    ScreenSetup::ScreenSetup(QWidget* _parent) :
      QWidget(_parent)
    {
      setMouseTracking(true);
      setAcceptDrops(true);

      connect(QGuiApplication::instance(),SIGNAL(screenAdded(QScreen*)),
          this,SLOT(updateScreens()));
      connect(QGuiApplication::instance(),SIGNAL(screenRemoved(QScreen*)),
          this,SLOT(updateScreens()));

      updateScreens();
    }

    ScreenSetup::~ScreenSetup()
    {
      screenItems_.clear();
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
 
    void ScreenSetup::updateScreens()
    {
      screenItems_.clear();
      desktopRect_ = omni::proj::ScreenSetup::desktopRect();

      auto _screens = omni::proj::ScreenSetup::screens();
      for (auto& _screen : _screens)
      {
        //if (_screen != QGuiApplication::primaryScreen()) continue; 
        screenItems_[_screen].reset(new Item(*this,_screen));
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

      if (screenItems_.empty())
      {
        QFont _font("Helvetica",30 / devicePixelRatio());
        _p.setFont(_font);
        _p.setBrush(QBrush("#cccccc"));
        _p.drawText(rect(),Qt::AlignCenter,"No screen connected.");
        return;
      }
  
      _p.setPen(QPen(QColor("#ffffff").darker(150),3));
      _p.drawRect(transformedRect());

      /// Draw all items
      for (auto& _item : screenItems_)
      {
        _item.second->paint(_p);
      }
    }
      
    void ScreenSetup::mouseMoveEvent(QMouseEvent* _event)
    {
      bool _update = false;

      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        int _oldHoverIndex = _item->hoverIndex();
        _item->setHoverIndex(_event->pos());
        
        // Update only when hover index has changed
        _update |= _oldHoverIndex != _item->hoverIndex();
      }

      update();
    }
      
    void ScreenSetup::mouseDoubleClickEvent(QMouseEvent* _event)
    {
      auto _screenItem = getItemAtPos(_event->pos());
      if (_screenItem) 
      {
        _screenItem->setHoverIndex(_event->pos());
        _screenItem->detachCurrentTuning();
        update();
      }
    }
 
    ScreenSetup::Item* ScreenSetup::getItemAtPos(QPoint const& _pos)
    {
      Item* _itemAtPos = nullptr;
      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        if (_item->rect().contains(_pos)) 
        {
          _itemAtPos = _item.get();
          break;
        }
      }

      return _itemAtPos;
    }
 
    void ScreenSetup::dragEnterEvent(QDragEnterEvent* event)
    {
      auto* _screenItem = getItemAtPos(event->pos());
      for (auto& _item : screenItems_)
        _item.second->setDrop(false);
 
      if (event->mimeData()->hasFormat("text/plain"))
      {
        if (_screenItem) 
        {
          auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
          if (!_tuningWidget->tuning()) return;

          auto _color = _tuningWidget->tuning()->color();
          _screenItem->setDrop(true,_color);           
        }

        event->acceptProposedAction();
      }
      update();
    }

    void ScreenSetup::dragMoveEvent(QDragMoveEvent* event)
    {
      auto* _screenItem = getItemAtPos(event->pos());
 
      for (auto& _item : screenItems_)
        _item.second->setDrop(false);
  
      if (event->mimeData()->hasFormat("text/plain") && _screenItem)
      {
        
        auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
        if (!_tuningWidget->tuning()) return;
       
        auto _color = _tuningWidget->tuning()->color();
        _screenItem->setHoverIndex(event->pos());
        _screenItem->setDrop(true,_color);
        event->acceptProposedAction();
      }
      update();
    }

    void ScreenSetup::dropEvent(QDropEvent* event)
    { 
      for (auto& _item : screenItems_)
        _item.second->setDrop(false);

      auto* _screenItem = getItemAtPos(event->pos()); 
      //dragWidget_->hide();

      if (_screenItem) 
      {
        _screenItem->setDrop(false);
        _screenItem->setHoverIndex(event->pos());
        auto* _tuningWidget = static_cast<proj::Tuning*>(event->source());
        if (!_tuningWidget->tuning()) return;
   
        _screenItem->attachTuning(_tuningWidget); 
        event->acceptProposedAction();
      }

      for (auto& _screenItem : screenItems_)
      {
        auto& _item = _screenItem.second;
        _item->endDrop();
      }
    }

    /// ScreenSetup::SubScreenItem 
    /////////////////////////////////////////
    

    ScreenSetup::SubScreenItem::SubScreenItem() 
    {
    }

    ScreenSetup::SubScreenItem::SubScreenItem(Item const* _parent, int _index) : 
      parent_(_parent),
      index_(_index)
    {
    }

    QRect ScreenSetup::SubScreenItem::rect() const
    {
      auto _r = parent_->rect(); // Parent rect
      int _width = _r.width() / omni::proj::ScreenSetup::subScreenCount(parent_->screen());
      return QRect(_r.x() + index_ * _width,_r.y(),_width,_r.height());
    }
        
    QSize ScreenSetup::SubScreenItem::size() const
    {
      auto _screen = parent_->screen();
      return QSize(
          omni::proj::ScreenSetup::subScreenWidth(_screen),
          _screen->size().height());
    }

    void ScreenSetup::SubScreenItem::paint(bool _hover, bool _drop, QColor _dropColor, QPainter& _p)
    {
      // Draw rectangle with tuning color
      QColor _color = tuning_ != nullptr ? tuning_->tuning()->color() : "#cccccc";

      if (_hover)
      {
        _color = _color.lighter(120);
      }

      _p.setBrush(_color);
      _p.setPen(QPen(_color.darker(200),1));

      auto _rect = rect();
      _p.drawRect(_rect); 

      /// Draw diagonal stripes for drop
      if (_drop)
      {
        QPixmap _pixmap(32,32);
        _pixmap.fill(_color);
        QPainter _texP(&_pixmap);
        _texP.setPen(QPen(_dropColor,2));
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
      QString _resStr = QString("%1 x %2").arg(size().width()).arg(size().height());

      _p.drawText(_rect,Qt::AlignHCenter | Qt::AlignVCenter,_resStr);
    }
            
    omni::ui::proj::Tuning* ScreenSetup::SubScreenItem::tuning()
    {
      return tuning_;
    }

    omni::ui::proj::Tuning const* ScreenSetup::SubScreenItem::tuning() const
    {
      return tuning_;
    }

    void ScreenSetup::SubScreenItem::setTuning(omni::ui::proj::Tuning* _tuning)
    {
      tuning_ = _tuning;
    }


    /// END SubScreenItem
    /////////////////////////////////////////


    /// ScreenSetup::Item 
    /////////////////////////////////////////
    
    ScreenSetup::Item::Item(
        ScreenSetup& _screenSetup,
        QScreen const* _screen) :
      screenSetup_(_screenSetup),
      screen_(_screen),
      fullscreen_(new FullScreen(_screen))
    {
      int _numScreens = omni::proj::ScreenSetup::subScreenCount(_screen);
      for (int i = 0; i < _numScreens; ++i)
      {
        subScreens_.emplace_back(this,i);
      }
    }

    ScreenSetup::Item::~Item()
    {
      detachTunings();
      delete fullscreen_;
    }

    QScreen const* ScreenSetup::Item::screen() const
    {
      return screen_;
    }

    int ScreenSetup::Item::hoverIndex() const
    {
      return hoverIndex_;
    }

    void ScreenSetup::Item::setHoverIndex(int _hoverIndex) 
    {
      hoverIndex_ = _hoverIndex;
    }
    
    /// Set hover index from mouse position
    void ScreenSetup::Item::setHoverIndex(QPoint const& _pos)
    {
      for (int i = 0; i < subScreens_.size(); ++i)
      {
        if (subScreens_[i].rect().contains(_pos)) 
        {
          hoverIndex_ = i;
          return;
        }
      }

      hoverIndex_ = -1;
    }

    bool ScreenSetup::Item::drop() const
    {
      return drop_;
    }

    void ScreenSetup::Item::setDrop(bool _drop, QColor const& _color) 
    {
      drop_=_drop;
      dropColor_ = _color;
      if (_drop)
      {
        fullscreen_->showDragWidget(hoverIndex_,_color);
      } else
      {
        hoverIndex_ = -1;
      }
    }
 
    /// Hides fullscreen widget
    void ScreenSetup::Item::endDrop()
    {
      fullscreen_->hideDragWidget();
    }

    QRect ScreenSetup::Item::rect() const
    {
      return screenSetup_.transformedRect(screen_->geometry()).toRect();
    }

    void ScreenSetup::Item::paint(QPainter& _p)
    {
      int i = 0;
      for (auto& _subScreen : subScreens_)
      {
        bool _hover = i == hoverIndex_;
        _subScreen.paint(
            _hover,
            drop_ && _hover,
            dropColor_,
            _p);
        ++i;  
      }

      if (subScreens_.size() > 1)
      {
        auto _rect = rect();
        // Resolution string
        QString _resStr = QString("%1 x %2").arg(screen_->size().width()).arg(screen_->size().height());
        _p.drawText(_rect,Qt::AlignHCenter,_resStr);
      }
    }

    void ScreenSetup::Item::attachTuning(omni::ui::proj::Tuning* _tuning)
    {
      if (hoverIndex_ < 0 || hoverIndex_ >= subScreens_.size()) return;

      _tuning->attachScreen(screen_,hoverIndex_);
      fullscreen_->attach(hoverIndex_,_tuning->fullscreenWidget());

      for (auto& _subScreen : subScreens_) 
        if (_subScreen.tuning() == _tuning)
          _subScreen.setTuning(nullptr);

      subScreens_[hoverIndex_].setTuning(_tuning);
      screenSetup_.update();
    }

    void ScreenSetup::Item::detachCurrentTuning()
    {
      auto _subScreen = subScreens_[hoverIndex_];
      if (!_subScreen.tuning()) return;

      fullscreen_->detach(hoverIndex_);
      _subScreen.tuning()->detachScreen();
      _subScreen.setTuning(nullptr);
      screenSetup_.update();
    }

    void ScreenSetup::Item::detachTunings()
    {
      int i = 0;
      for (auto& _subScreen : subScreens_)
      {
        if (!_subScreen.tuning()) continue;
        fullscreen_->detach(i);
        _subScreen.tuning()->detachScreen();
        _subScreen.setTuning(nullptr);
        ++i;
      }
      screenSetup_.update();
    }
  }
}

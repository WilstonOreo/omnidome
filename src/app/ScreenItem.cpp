#include <omni/ui/ScreenItem.h>

#include <QPainter>
#include <QGraphicsDropShadowEffect>

namespace omni
{
  namespace ui
  {
    ScreenItem::ScreenItem(Screen const& _screen, QGraphicsItem* _parent) :
      QGraphicsItem(_parent),
      screen_(_screen) 
    {
      setFlag(ItemSendsGeometryChanges);
      setFlag(ItemIsSelectable,true);
      setAcceptDrops(true);
      setCacheMode(DeviceCoordinateCache);

      QGraphicsDropShadowEffect* _effect = new QGraphicsDropShadowEffect();
      _effect->setBlurRadius(5.0);
      setGraphicsEffect(_effect);
    }

    ScreenItem::~ScreenItem()
    {
    }

    
    QRectF ScreenItem::boundingRect() const
    {
      return screen_.rect();
    }

    void ScreenItem::paint(QPainter* _painter, const QStyleOptionGraphicsItem* _style, QWidget*)
    {
      auto _rect = this->boundingRect();  
      _painter->setBrush(QBrush("#FF0000"));
      _painter->drawRect(_rect);
    }


    void ScreenItem::attachProjector(Projector const* _projector)
    {
      projector_=_projector;
      update();
    }

    void ScreenItem::detachProjector()
    {
      projector_ = nullptr;
      update();
    }

    Projector const* ScreenItem::projector() const 
    {
      return projector_;
    }
  }
}

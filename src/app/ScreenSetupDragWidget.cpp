#include <omni/ui/ScreenSetupDragWidget.h>

#include <QPainter>

namespace omni
{
  namespace ui
  {
    ScreenSetupDragWidget::ScreenSetupDragWidget(QWidget* _parent) :
      QWidget(_parent)
    {
      setWindowFlags( Qt::CustomizeWindowHint);
      setWindowFlags(Qt::FramelessWindowHint);
      setFocusPolicy(Qt::NoFocus);
      hide();
    }

    ScreenSetupDragWidget::~ScreenSetupDragWidget()
    {
    }

    QColor ScreenSetupDragWidget::color() const
    {
      return color_;
    }

    void ScreenSetupDragWidget::setColor(QColor const& _color)
    {
      color_=_color;
      update();
    }
 
    void ScreenSetupDragWidget::setRect(QRectF const& _rect)
    {
      setGeometry(_rect.toRect());
      raise();
    }
 
    void ScreenSetupDragWidget::paintEvent(QPaintEvent*)
    {
      QPainter _p(this);
      _p.setPen(Qt::NoPen);
      _p.setBrush(color());
      _p.drawRect(rect());
    }
  }
}

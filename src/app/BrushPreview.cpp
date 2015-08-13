#include <omni/ui/BrushPreview.h>

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <omni/BlendBrush.h>

using namespace omni::ui;

BrushPreview::BrushPreview(QWidget* _parent) :
  QWidget(_parent)
{
  update(brush_.feather(),brush_.invert());
  setMinimumSize(64 / devicePixelRatio(),64 / devicePixelRatio());
  setMaximumSize(128 / devicePixelRatio(),128 / devicePixelRatio());
}
      
void BrushPreview::update(float _feather, bool _invert)
{
  brush_.setBrush(std::min(width(),height()),_feather,_invert);
  image_ = brush_.buffer().toQImage();

  if (_invert)
    image_.invertPixels();

  QWidget::update();
}
      

void BrushPreview::resizeEvent(QResizeEvent* event)
{
  update(brush_.feather(),brush_.invert());
}

void BrushPreview::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.drawImage(0, 0,  image_);
}


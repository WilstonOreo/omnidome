#include <omni/ui/BrushPreview.h>

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <omni/BlendBrush.h>

using namespace omni::ui;

BrushPreview::BrushPreview(QWidget* _parent) :
  QWidget(_parent),
  image_(new QImage(64,64,QImage::Format_RGB32))
{}

void BrushPreview::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  auto&& _buf = blend::Mask::generateBrush(64,feather_);
  using namespace gex::raster;
  for_each(_buf,strategy::for_each::Pixel(),[&](const uint8_t& _pix, size_t _x, size_t _y)
  {
    int _v = invert_ ? 255 -  _pix : _pix;
    image_->setPixel(_x,_y,QColor(_v,_v,_v).rgb());
  });
  painter.drawImage(0, 0, *image_);
  QWidget::paintEvent(event);
}


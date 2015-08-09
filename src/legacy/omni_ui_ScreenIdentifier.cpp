#include <omni/ui/ScreenIdentifier.h>

#include <QPainter>

using namespace omni::ui;

ScreenIdentifier::ScreenIdentifier(size_t _index, size_t _subIndex) :
  QWidget(nullptr),
  index_(_index),
  subIndex_(_subIndex)
{
  setWindowFlags( Qt::CustomizeWindowHint );
  hide();
}

ScreenIdentifier::~ScreenIdentifier()
{
  setParent(nullptr);
}

void ScreenIdentifier::paintEvent ( QPaintEvent * event )
{
  QPainter _p(this);

  auto drawCenteredText = [&](size_t _height, const QPointF& _pos, const QString& _text)
  {
    QPainterPath _path;
    QFont _font("Helvetica",_height,QFont::Bold);
    _path.addText(0,0,_font,_text);
    QPointF _pathCenter(_path.boundingRect().center());
    _path.translate(_pos - _pathCenter);
    _p.drawPath(_path);
  };

  _p.setPen(QPen(QColor(0,0,0,255),2));
  _p.setBrush(QColor(255,255,255,255));

  for (int x = 0; x < 10; x++)
  {
    for (int y = 0; y < 10; y++)
    {
      QPoint _pos( x * width() /10,y * height() / 10 + (x % 2) *width() / 20);
      drawCenteredText(height()/15,_pos,QString("%1").arg(subIndex_+1));
    }
  }

  drawCenteredText(height()/3,QPointF(width()/2,height()/2),QString("%1").arg(index_+1));
}

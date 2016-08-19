
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPolygon>

namespace omni {

  class BlendMaskGenerator {
  public:
    void draw(
      QPainterPath const& _inner, QPainterPath const& _outer, QImage& _image)
      const {

      QImage _maskImage(_image);

      QPainter _p;
      _p.begin(&_maskImage);

      _p.setPen(Qt::NoPen);
      _p.setBrush(QBrush("#000000"));
      _p.drawRect(0,0,_image.width(),_image.height());

      _p.setBrush(QBrush("#00FF00"));
      _p.drawPath(_outer);

      _p.setBrush(QBrush("#FF0000"));
      _p.drawPath(_inner);

      uchar* _bits = _maskImage.bits();

      for (int y = 0; y < _maskImage.height(); ++y) {
        uchar *_line = _maskImage.scanLine(y);

        for (int x = 0; x < _maskImage.width(); ++x) {
          float _distance = getDistance(_bits,_maskImage.size(),QPoint(x,y));

          int _v = 255.0*qBound(0.0f,_distance,1.0f);
          _line[x*4+0] = _v;
          _line[x*4+1] = _v;
          _line[x*4+2] = _v;
          _line[x*4+3] = 255;
        }
      }

      _p.end();
    }

  private:
    float getDistance(
      uchar* _bits,
      QSize const& _size,
      QPoint const& _pos
    ) const {
      int _pos = (_size.width()*_pos.y()+_pos.x())*4;
      auto _red = _bits[_pos+2];
      auto _green = _bits[_pos+1];
      auto _blue = _bits[_pos+0];
      auto _alpha = _bits[_pos+3];


    }
  };
}

int main(int ac, char *av[])
{
  using namespace omni;

  BlendMaskGenerator _blendMaskGen;

  QImage _image(QSize(1024,1024),QImage::Format_ARGB32);

  QPolygon _innerPoly;
  _innerPoly << QPoint(256,256)
    << QPoint(768,256)
    << QPoint(768,768)
    << QPoint(256,768);

  QPainterPath _inner;
  _inner.addPolygon(_innerPoly);

  QPainterPath _outer;
  QPolygon _outerPoly;
  _outerPoly << QPoint(128,128)
      << QPoint(896,128)
      << QPoint(896,896)
      << QPoint(128,896);
  _outer.addPolygon(_outerPoly);

  _blendMaskGen.draw(_inner,_outer,_image);

  _image.save("blendmask.png");

  return EXIT_SUCCESS;
}

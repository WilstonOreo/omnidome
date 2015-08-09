#include <omni/View.h>

#include <omni/proj/Screen.hpp>

#ifndef OMNI_NO_GUI
#include <QApplication>
#include <QImage>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <omni/ui/util.hpp>
#endif
#include <omni/debug.hpp>

using namespace omni;

View::View(const Color4f& _color, size_t _id) :
  blendMask_(this),
  id_(_id),
  screen_(nullptr),
  subScreenId_(0),
  color_(_color)
{
  resize(1024,768);
  blendMask_.update();
}

bool View::resolutionToBeChanged(proj::Screen const* _screen) const
{
  if (_screen)
  {
    return (_screen->width() != width() || _screen->height() != height()) && (_screen != screen());
  }
  else
  {
    return projector_.width() != width() || projector_.height() != height();
  }
  return false;
}



void View::screen(proj::Screen const* _screen)
{
  if (resolutionToBeChanged(_screen)) blendMask_.clear();
  screen_=_screen;
  subScreenId_ = 0;
  blendMask_.clear();
  generateProjectorImage();
}

bool View::hasScreen() const
{
  return screen_ != nullptr;
}

size_t View::width() const
{
  return hasScreen() ? screen()->subScreenWidth() :  projector_.width();
}

size_t View::height() const
{
  return hasScreen() ? screen()->height() :  projector_.height();
}

Scalar View::aspectRatio() const
{
  return Scalar(width()) / height();
}

void View::resize(size_t _width, size_t _height)
{
  if (_width == width() && _height == height()) return;

  if (!screen())
  {
    projector_.resize(_width,_height);
  }
  OMNI_DBG("View::resize " << _width << "\t" << _height);

  blendMask_.clear();
  generateProjectorImage();
}

bool View::load(const ConfigPath& _path, Config const& _config)
{
  bool _updated = Serializer<View>::load(_path,_config);
  if (_updated) generateProjectorImage();
  return _updated;
}

void View::generateProjectorImage()
{
  auto& _img = projectorImage_;
  _img.resize(width(),height());
#ifndef OMNI_NO_GUI
  if (!QApplication::instance()) return;
  QPainter _p;
  QImage _qImage(_img.width(),_img.height(),QImage::Format_ARGB32);
  _p.begin(&_qImage);
  _p.setPen(Qt::NoPen);
  _p.setBrush(QBrush(QColor(color_.r()*255,color_.g()*255,color_.b()*255,255)));
  _p.drawRect(QRect(0,0,_img.width(),_img.height()));
  _p.setPen(QColor(0,0,0,255));
  _p.setBrush(Qt::NoBrush);

  for (int i = 0; i < _img.width(); i += 32)
  {
    _p.drawLine(i,0,i,_img.height());
  }
  for (int i = 0; i < _img.height(); i += 32)
  {
    _p.drawLine(0,i,_img.width(),i);
  }

  _p.setPen(QPen(QColor(0,0,0,255),2));
  _p.setBrush(QColor(0,0,0,255));
  /// Draw Ellipses in each corner
  auto ellipse = [&_p](float _r, size_t _x, size_t _y)
  {
    _p.drawEllipse(_x-_r,_y-_r,_r*2,_r*2);
    _p.drawLine(_x-_r*2,_y,_x+_r*2,_y);
    _p.drawLine(_x,_y-_r*2,_x,_y+_r*2);
  };

  ellipse(16,0,0);
  ellipse(16,0,_img.height()-1);
  ellipse(16,_img.width()-1,0);
  ellipse(16,_img.width()-1,_img.height()-1);
  ellipse(12,_img.width()/2,0);
  ellipse(12,0,_img.height()/2);
  ellipse(12,_img.width()/2,_img.height());
  ellipse(12,_img.width(),_img.height()/2);

  QPainterPath _path;
  QFont _font("Helvetica",_img.height()/3,QFont::Bold);
  _path.addText(0,0,_font,QString("%1").arg(id_+1));

  QPointF _pathCenter(_path.boundingRect().center());
  QPointF _imageCenter(_img.width()/2,_img.height()/2);
  _path.translate(_imageCenter - _pathCenter);
  _p.setBrush(QColor(255,255,255,255));
  _p.drawPath(_path);

  adapt(_qImage,_img);
  _p.end();
#endif
}

Projector& View::projector()
{
  return projector_;
}

WarpGrid& View::warpGrid()
{
  return warpGrid_;
}

BlendMask& View::blendMask()
{
  return blendMask_;
}


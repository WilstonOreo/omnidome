/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <slim/GradientSlider.h>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

using namespace slim;

GradientSlider::GradientSlider(QWidget* _widget) : 
  Widget(_widget),
  mixin_range_type(0,0,1,0.01,0.1)
{
  Widget::label_->setAlignment(Qt::AlignCenter);
}

GradientSlider::GradientSlider(QString const& _label, QWidget* _widget) : 
  Widget(_label,_widget),
  mixin_range_type(0,0,1,0.01,0.1)
{
  Widget::label_->setAlignment(Qt::AlignCenter);
}

GradientSlider::~GradientSlider()
{
}
    
QLinearGradient& GradientSlider::gradient()
{
  return gradient_;
}

QLinearGradient const& GradientSlider::gradient() const
{
  return gradient_;
}

void GradientSlider::setGradient(QLinearGradient const& _gradient)
{
  gradient_=_gradient;
}

void GradientSlider::setShowColorOnly(bool _showColorOnly)
{
  showColorOnly_ = _showColorOnly;
  update();
}

void GradientSlider::setValue(double _value)
{
  mixin_range_type::setValue(_value);
  update();
  emit valueChanged();
}

    
QColor GradientSlider::color() const
{
  QImage _image(width(),1,QImage::Format_ARGB32);
  QPainter _p(&_image);

  _p.setPen(Qt::NoPen);
  _p.setBrush(gradient_);
  _p.drawRect(QRect(0,0,width(),1));
  _p.end();

  double _pos = valueToPos();
  if (_pos <  0.0) _pos = 0.0;
  if (_pos >= width()-1) _pos = width()-1;

  return _image.pixel(_pos,0);
}

bool GradientSlider::showColorOnly() const
{
  return showColorOnly_;
  
}
void GradientSlider::paintEvent(QPaintEvent* e)
{
  QPainter _p(this);


  auto _rect = rect().adjusted(1,1,-1,-1);
  _p.setPen(Qt::NoPen);
  
  if (showColorOnly() && !moving_)
  {
    _p.setBrush(color());
  } else
  {
    _p.setBrush(gradient_);
  }

  _p.drawRect(_rect);

  if (moving_)
  {
    _p.setPen(Qt::NoPen);
    _p.setBrush(color());
    _p.drawRect(QRect(1,1,width()-1,height()/6.0-1));
    _p.drawRect(QRect(1,5.0*height()/6.0+1,width()-1,height()/6.0-1));
  }
  
  Widget::drawBorder(_p);

  // Draw Handle
  if (!showColorOnly() || moving_)
  {
  double _pos = valueToPos();

  QRectF _progressRect = rect();

  _progressRect.setRight(_pos);
  _p.setPen(QPen(colorSet().dark(),4));
  _p.drawLine(
      QPointF(_pos,_progressRect.bottom()+2),
      QPointF(_pos,_progressRect.top()- 2));
  }

}

double GradientSlider::valueToPos() const
{
  return valueToPos(value());
}

/// Calculate position from value
double GradientSlider::valueToPos(double _value) const
{
  auto&& _rect = rect();
  return _rect.left() + mixin::Range<double>::ratio(_value)* double(_rect.width());
}

void GradientSlider::mouseMoveEvent(QMouseEvent* e)
{
  if (moving_)
  {
    setValue(valueFromPos(e->pos().x()));
  }
}
    
void GradientSlider::mousePressEvent(QMouseEvent* e)
{
  moving_ = true;
  setValue(valueFromPos(e->pos().x()));
}

void GradientSlider::mouseReleaseEvent(QMouseEvent* e)
{
  if (moving_)
  {
    setValue(valueFromPos(e->pos().x()));
    update();
  }
  moving_ = false;
}

void GradientSlider::wheelEvent(QWheelEvent* e)
{
  if (hasFocus())
    setValue(value() + e->delta()/15.0 / 8.0 * singleStep());
}

double GradientSlider::valueFromPos(double _pos) const
{
  auto&& _rect = rect();
  return (_pos - _rect.left()) / _rect.width() * (maximum() - minimum()) + minimum();
}


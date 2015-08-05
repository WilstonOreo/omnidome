#include <slim/RotationDial.h>

#include <cmath>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QVector2D>

using namespace slim;

RotationDial::RotationDial(QWidget* _parent) :
  QWidget(_parent),
  mixin_range_type(0,360),
  mixin_step_type(5,45),
  colorSet_(this)
{
  init();
}

RotationDial::RotationDial(double _x, double _y, double _z, QWidget* _parent) :
  QWidget(_parent),
  mixin_range_type(0,360),
  mixin_step_type(5,45),
  colorSet_(this)
{
  init(_x,_y,_z);
}

RotationDial::~RotationDial()
{
}


bool RotationDial::showTicks() const
{
  return showTicks_;
}

bool RotationDial::isMoving() const
{
  return moveState_ != MOVE_NONE;
}

void RotationDial::setShowTicks(bool _showTicks)
{
  showTicks_=_showTicks;
  update();
}

void RotationDial::init(double _x, double _y, double _z)
{
  setMinimumSize(0,64);

  valueLabel_ = new QLabel(this);
  valueLabel_->setAlignment(Qt::AlignCenter);
  valueLabel_->setStyleSheet("background : transparent");

  setX(_x);
  setY(_y);
  setZ(_z);
}

double RotationDial::x() const
{
  return x_;
}

double RotationDial::y() const
{
  return y_;
}

double RotationDial::z() const
{
  return z_;
}

QColor RotationDial::xColor() const
{
  return xColor_;
}

QColor RotationDial::yColor() const
{
  return yColor_;
}

QColor RotationDial::zColor() const
{
  return zColor_;
}

void RotationDial::setX(double _x)
{
  x_=_x;
  emit xChanged();
  update();
}

void RotationDial::setY(double _y)
{
  y_=_y;
  emit yChanged();
  update();
}

void RotationDial::setZ(double _z)
{
  z_=_z;
  emit zChanged();
  update();
}

void RotationDial::setXColor(QColor _xColor)
{
  xColor_=_xColor;
}

void RotationDial::setYColor(QColor _yColor)
{
  yColor_=_yColor;
}

void RotationDial::setZColor(QColor _zColor)
{
  zColor_=_zColor;
}

ColorSet const& RotationDial::colorSet() const
{
  return colorSet_;
}

void RotationDial::valueChangedEvent()
{
  update();
}


void RotationDial::resizeEvent(QResizeEvent* e)
{
  valueLabel_->setGeometry(rect());
}


void RotationDial::mousePressEvent(QMouseEvent* e)
{
  setFocus();

  int _circle =  QVector2D(rect().center() - e->pos()).length() / radius() * 5;

  switch (_circle)
  {
  case 2:
    moveState_ = MOVE_Z;
    break;
  case 3:
    moveState_ = MOVE_Y;
    break;
  case 4:
    moveState_ = MOVE_X;
    break;
  default:
    moveState_= MOVE_NONE;
  }

  if (moveState_ != MOVE_NONE)
  {
    pos_ = e->pos();
    setValue(getValue(e->pos()));
  }
}

void RotationDial::mouseReleaseEvent(QMouseEvent* e)
{
  if (moveState_ != MOVE_NONE)
  {
    pos_ = e->pos();
    setValue(getValue(e->pos()));
  }
  moveState_ = MOVE_NONE;
  valueLabel_->setText("");
}

void RotationDial::mouseMoveEvent(QMouseEvent* e)
{
  if (moveState_ != MOVE_NONE)
  {
    pos_ = e->pos();
    setValue(getValue(e->pos()));
  }

}

QString RotationDial::formatDegrees(double _val)
{
  return QString::number(_val,'f',1) + QString("°");
}

QString RotationDial::moveStateToText(MoveState _moveState)
{
  switch (_moveState)
  {
  case MOVE_X:
    return "X";
    break;
  case MOVE_Y:
    return "Y";
    break;
  case MOVE_Z:
    return "Z";
    break;
  default:
    break;
  }
  return "";
}

void RotationDial::setValue(double _val)
{
  if (isMoving())
    valueLabel_->setText(formatDegrees(_val));

  switch (moveState_)
  {
  case MOVE_X:
    setX(_val);
    break;
  case MOVE_Y:
    setY(_val);
    break;
  case MOVE_Z:
    setZ(_val);
    break;
  case MOVE_NONE:
  default:
    break;
  }
}

double RotationDial::getValue(QPoint const& _point)
{
  auto _v = QVector2D(_point - rect().center()).normalized();
  float _angle = atan2(_v.x(),-_v.y()) / M_PI * 0.5;
  _angle = (_angle + 0.5)* (maximum() - minimum()) + minimum();
  return snap() ? int(_angle / singleStep() + 0.5)*singleStep()  : _angle;
}

#include <QDebug>

void RotationDial::paintPies(QPainter& _p)
{
  double _r = radius() - radius() / 50.0;
  QColor _hColor = colorSet().highlight();

  paintPie(_p,_r * 0.9,_r * 1.0,x(),moveState_ == MOVE_X ? _hColor : xColor());
  paintPie(_p,_r * 0.7,_r * 0.8,y(),moveState_ == MOVE_Y ? _hColor : yColor());
  paintPie(_p,_r * 0.5,_r * 0.6,z(),moveState_ == MOVE_Z ? _hColor : zColor());
}

void RotationDial::paintPie(QPainter& _p, double _innerRadius, double _outerRadius, double _value, QColor _color)
{

  QPointF _center = rect().center();
  
  _p.setPen(Qt::NoPen);
  QPainterPath _ellipsePath, _piePath;
  _ellipsePath.addEllipse(_center,_outerRadius,_outerRadius);
  _ellipsePath.addEllipse(_center,_innerRadius,_innerRadius);
  auto&& _pieRect =_ellipsePath.boundingRect().adjusted(-2,-2,2,2);

  _p.setBrush(_color);

  /// Paint arc
  QPainterPath _arc(_center);
  _arc.arcTo(_pieRect,-90,-mixin_range_type::ratio(_value)*360);
  _arc.lineTo(_center);
  _arc.closeSubpath();

  _piePath = _arc & _ellipsePath;
  _p.drawPath(_piePath);


  // Paint border line of arc 
  _p.setPen(QPen(colorSet().shadow(),radius()/50.0));
  _p.setBrush(Qt::NoBrush);
  _p.drawEllipse(_center,_outerRadius,_outerRadius);

  double _width = (_outerRadius - _innerRadius);

  /// Paint ticks
  if (showTicks())
  {
    for_each_step(minimum(),maximum(),[&](int _step, double i, bool _isPage)
    {
      _p.setPen(QPen(i >= _value ?
                     colorSet().shadow() :
                     colorSet().windowText(),1));
      paintTick(_p,
                _isPage ? _innerRadius :
                _outerRadius - _width * 0.5,_outerRadius,i);
    });
  }

  _p.setPen(QPen(colorSet().shadow(),radius()/50.0));
  paintTick(_p,_outerRadius - _width * 2.0,_outerRadius,_value);
}

void RotationDial::paintEvent(QPaintEvent* e)
{
  auto&& _rect = rect();
  QPointF _center = _rect.center();


  double _r = radius() - radius() / 50.0;
  QPainter _p(this);
  _p.setRenderHint(QPainter::Antialiasing);
  paintPies(_p);

  _p.setPen(QPen(colorSet().windowText(),2));
  QFont _font = font();
  _font.setPointSize(_r*0.08);
  _p.setFont(_font);

  if (moveState_ != MOVE_X) _p.drawText(QRect(rect().left(),_center.y() - radius()*1.08,width(),_r*0.5) , Qt::AlignCenter, "X");
  if (moveState_ != MOVE_Y) _p.drawText(QRect(rect().left(),_center.y() - radius()*0.88,width(),_r*0.5) , Qt::AlignCenter, "Y");
  if (moveState_ != MOVE_Z) _p.drawText(QRect(rect().left(),_center.y() - radius()*0.68,width(),_r*0.5) , Qt::AlignCenter, "Z");

  _font = font();
  _font.setPointSize(_r*0.08);
  _p.setFont(_font);
  if (moveState_ != MOVE_X) _p.drawText(QRect(rect().left(),_center.y() + radius()*0.60,width(),_r*0.5) , Qt::AlignCenter, formatDegrees(x_));
  if (moveState_ != MOVE_Y) _p.drawText(QRect(rect().left(),_center.y() + radius()*0.40,width(),_r*0.5) , Qt::AlignCenter, formatDegrees(y_));
  if (moveState_ != MOVE_Z) _p.drawText(QRect(rect().left(),_center.y() + radius()*0.20,width(),_r*0.5) , Qt::AlignCenter, formatDegrees(z_));


  if (isMoving())
  {
    _p.setPen(QPen(colorSet().shadow(),2));
    QFont _font = font();
    _font.setPointSize(_r*0.4);
    _p.setFont(_font);

    _p.drawText(_rect, Qt::AlignCenter, moveStateToText(moveState_));
  }
}

double RotationDial::radius() const
{
  double _r = width() > height() ? height() : width();
  return _r * 0.5;

}

void RotationDial::paintTick(QPainter& _p, double _inner, double _outer, double _value)
{
  auto&& _rect = rect();
  auto _center = QVector2D(_rect.center());

  auto _theta = mixin_range_type::ratio(_value) * M_PI * 2.0;

  QVector2D _a(-sin(_theta),cos(_theta));
  QVector2D _b(_a);
  _a *= _inner;
  _b *= _outer;
  _a += _center;
  _b += _center;

  _p.drawLine(_a.toPointF(),_b.toPointF());
}


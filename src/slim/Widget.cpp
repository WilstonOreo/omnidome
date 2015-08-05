#include <slim/Widget.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

using namespace slim;

Widget::Widget(QWidget* _parent) :
  QWidget(_parent), 
  colorSet_(this)
{
  init();
}

Widget::Widget(QString const& _label, QWidget* _parent) :
  QWidget(_parent),
  colorSet_(this)
{
  init();
  setLabel(_label);
}

QString Widget::label() const
{
  return label_->text();
}

void Widget::setLabel(QString const& _label)
{
  label_->setText(_label);
  update();
}
    
ColorSet const& Widget::colorSet() const
{
  return colorSet_;
}

void Widget::init()
{
  setWindowFlags(Qt::FramelessWindowHint);
  setMinimumSize(0,32 / devicePixelRatio());
  setMaximumSize(16000,48);
  setFocusPolicy(Qt::TabFocus);
  label_ = new QLabel("",this);
  label_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  label_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
  label_->setStyleSheet("background : transparent");
  label_->setPalette(this->palette());
  
  QLayout* _layout = new QHBoxLayout(this);
  _layout->setContentsMargins(4,1,4,1);
  _layout->addWidget(label_);
  setLayout(_layout);
}

void Widget::drawBorder(QPainter& _p)
{
  drawBorder(_p,hasFocus());
}

void Widget::drawBorder(QPainter& _p, bool _focus)
{
  auto&& _rect = rect();

  QStyleOption _option;
  _option.initFrom(this);

  _p.setPen(_focus ? QPen(QBrush(_option.palette.color(colorSet().colorGroup(),QPalette::Dark)),2) : Qt::NoPen);
  _p.setBrush(Qt::NoBrush);
  _p.drawRoundedRect(_rect,4,4);
}        

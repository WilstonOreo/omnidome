#include <slim/SpanFloat.h>

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

using namespace slim;

SpanFloat::SpanFloat(QWidget* _parent) :
  AbstractInputWidget(_parent)
{
  init();
}

SpanFloat::SpanFloat(QString const& _label,
  double _lower, double _upper, double _min, double _max, QWidget* _parent) :
  AbstractInputWidget(_parent),
  mixin_span_type(_lower,_upper,_min,_max)
{
  setLabel(_label);
  init();
}

SpanFloat::~SpanFloat()
{
}

void SpanFloat::lowerChangedEvent()
{
  mixin_span_type::apply(editorAs<editor_type>(),lowerValue());
  AbstractInputWidget::valueLabel_->setText(QString("%1").arg(lowerValue()));
  update();
  emit lowerChanged();
}

void SpanFloat::upperChangedEvent()
{
  mixin_span_type::apply(editorAs<editor_type>(),lowerValue());
  AbstractInputWidget::valueLabel_->setText(QString("%1").arg(lowerValue()));
  update();
  emit lowerChanged();
}
  
void SpanFloat::rangeChangedEvent()
{
  mixin_span_type::apply(editorAs<editor_type>());
  emit rangeChanged();
}

void SpanFloat::setLowerValue(double _value)
{
  mixin_span_type::setLowerValue(_value);
}

void SpanFloat::setUpperValue(double _value)
{
  mixin_span_type::setUpperValue(_value);
}

void SpanFloat::setMinimum(double _min)
{
  mixin_span_type::setMinimum(_min);
}
void SpanFloat::setMaximum(double _max)
{
  mixin_span_type::setMaximum(_max);
}

void SpanFloat::setRange(double _min, double _max)
{
  mixin_span_type::setRange(_min,_max);
}

double SpanFloat::fromPos(double _pos) const
{
  auto&& _rect = rect();
  return (_pos - _rect.left()) / _rect.width() * range() + minimum();
}


double SpanFloat::upperToPos() const
{
  auto&& _rect = rect();
  return _rect.left() + Range<double>::ratio(upperValue())* float(_rect.width());
}

double SpanFloat::lowerToPos() const
{
  auto&& _rect = rect();
  return _rect.left() + Range<double>::ratio(lowerValue())* float(_rect.width());
}

void SpanFloat::paintEvent(QPaintEvent* _paintEvent)
{
  QPainter _p(this);
  _p.setRenderHint(QPainter::Antialiasing);
  double _lowerPos = lowerToPos();
  double _upperPos = upperToPos();
  
  AbstractInputWidget::drawTrack(_p,_lowerPos,_upperPos);
  AbstractInputWidget::drawHandle(_p,_lowerPos);
  AbstractInputWidget::drawHandle(_p,_upperPos);
  AbstractInputWidget::drawBorder(_p,hasFocus() || editor_->hasFocus());
}

void SpanFloat::mouseMoveEvent(QMouseEvent* e)
{
  if (moveState_ == LOWER_MOVING)
  {
    setLowerValue(fromPos(e->pos().x()));
  }
  
  if (moveState_ == UPPER_MOVING)
  {
    setUpperValue(fromPos(e->pos().x()));
  }
}

void SpanFloat::mousePressEvent(QMouseEvent* e)
{
  AbstractInputWidget::mousePressEvent(e);
  if (std::abs(e->pos().x() - lowerToPos()) <= 4)
  {
    moveState_ = LOWER_MOVING;
    setLowerValue(fromPos(e->pos().x()));
    return;
  }
  
  if (std::abs(e->pos().x() - upperToPos()) <= 4)
  {
    moveState_ = UPPER_MOVING;
    setUpperValue(fromPos(e->pos().x()));
    return;
  }
}

void SpanFloat::mouseReleaseEvent(QMouseEvent* e)
{
  moveState_ = NONE;
}
        
void SpanFloat::setLowerEditorVisible(bool _visible)
{
  AbstractInputWidget::setEditorVisible(_visible);
  mixin_span_type::apply(editorAs<editor_type>(),lowerValue());
}

void SpanFloat::setUpperEditorVisible(bool _visible)
{
  AbstractInputWidget::setEditorVisible(_visible);
  mixin_span_type::apply(editorAs<editor_type>(),upperValue());
}

void SpanFloat::init()
{
  moveState_ = NONE;
  setSingleStep(1.0);

  auto _editor = AbstractInputWidget::createEditor<editor_type>();
 
  _editor->setFrame(false);
  _editor->setAlignment(Qt::AlignRight);
  _editor->setButtonSymbols(QAbstractSpinBox::PlusMinus);
  mixin_span_type::apply(_editor);
  lowerChangedEvent();
  upperChangedEvent();
  hideEditor();
}


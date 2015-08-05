#include <slim/String.h>


#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

using namespace slim;

String::String(QWidget* _parent) :
  AbstractInputWidget(_parent)
{
  init();
}

String::String(QString const& _label,
  QString const& _text, QWidget* _parent) :
  AbstractInputWidget(_parent)
{
  setLabel(_label);
  init();
  setValue(_text);
}

String::~String()
{
}

QString String::value() const
{
  return editorAs<editor_type>()->text();
}
 
void String::setValue(QString const& _value)
{
  editorAs<editor_type>()->setText(_value);
  AbstractInputWidget::valueLabel_->setText(editorAs<editor_type>()->text());
  update();
  emit valueChanged();
}

void String::paintEvent(QPaintEvent* _paintEvent)
{
  QPainter _p(this);
  auto&& _rect = rect();

  _p.setRenderHint(QPainter::Antialiasing);

  _p.setBrush(colorSet().highlight());
  _p.drawRoundedRect(_rect,4,4);

  Widget::drawBorder(_p,hasFocus() || editor_->hasFocus());
}

void String::mousePressEvent(QMouseEvent* e)
{
  AbstractInputWidget::mouseDoubleClickEvent(e);
}
        
void String::setEditorVisible(bool _visible)
{
  AbstractInputWidget::setEditorVisible(_visible);
  AbstractInputWidget::valueLabel_->setText(editorAs<editor_type>()->text());
}

void String::init()
{
  AbstractInputWidget::init();
  auto _editor = AbstractInputWidget::createEditor<editor_type>();
 
  connect(_editor,SIGNAL(textChanged(QString const&)),this,SLOT(setValue(QString const&)));
  _editor->setFrame(false);
  _editor->setAlignment(Qt::AlignRight);
  hideEditor();
}


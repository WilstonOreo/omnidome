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


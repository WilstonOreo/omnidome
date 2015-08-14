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

#include <slim/AbstractInputWidget.h>

#include <QKeyEvent>
#include <QPainter>
#include <QStyleOption>

using namespace slim;

AbstractInputWidget::AbstractInputWidget(QWidget* _parent) :
  Widget(_parent),
  editor_(nullptr),
  valueLabel_(nullptr)
{
  init();
}

void AbstractInputWidget::init()
{
  valueLabel_ = new QLabel("",this);
  valueLabel_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  valueLabel_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
  valueLabel_->setStyleSheet("background : transparent");
  layout()->addWidget(valueLabel_);
}

void AbstractInputWidget::setEditorVisible(bool _visible)
{
  if (!editor_) return;
  editor_->setVisible(_visible);
  valueLabel_->setVisible(!_visible);
  if (!_visible) 
  {
    setFocus();
    return; 
  }
  editor_->setFocus();
}

void AbstractInputWidget::showEditor()
{
  setEditorVisible(true);
}

void AbstractInputWidget::hideEditor()
{
  setEditorVisible(false);
}

void AbstractInputWidget::mousePressEvent(QMouseEvent*)
{
  setFocus();
  hideEditor();
}

void AbstractInputWidget::mouseDoubleClickEvent(QMouseEvent*)
{
  showEditor();
}
 
void AbstractInputWidget::keyPressEvent(QKeyEvent* e)
{
  if ((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return))
  {
    setEditorVisible(!editor_->hasFocus());
  }
}

void AbstractInputWidget::drawHandle(QPainter& _p, double _pos, double _heightFactor) const
{ 
  QRectF _progressRect = rect();
  
  _progressRect.setRight(_pos);
  _p.setPen(QPen(colorSet().dark(),4));
  _p.drawLine(
      QPointF(_pos,_progressRect.bottom()+2),
      QPointF(_pos,_progressRect.bottom()- _heightFactor * _progressRect.height() - 2));
}

void AbstractInputWidget::drawTrack(QPainter& _p, double _left, double _right) const
{
  QRectF _progressRect = rect();
  _p.setBrush(QBrush(colorSet().window()));
  _p.drawRoundedRect(_progressRect,4,4);

  if (_left > _right) std::swap(_left,_right);

  _progressRect.setLeft(_left);
  _progressRect.setRight(_right);
  _p.setBrush(QBrush(colorSet().highlight()));
  _p.drawRect(_progressRect);
}
    
void AbstractInputWidget::drawTrack(QPainter& _p, double _pos) const
{
  drawTrack(_p,rect().left(),_pos);
}





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

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

#include <omni/ui/ScreenSetupDragWidget.h>

#include <QPainter>

namespace omni
{
  namespace ui
  {
    ScreenSetupDragWidget::ScreenSetupDragWidget(QWidget* _parent) :
      QWidget(_parent)
    {
      setWindowFlags( Qt::CustomizeWindowHint);
      setWindowFlags(Qt::FramelessWindowHint);
      setFocusPolicy(Qt::NoFocus);
      hide();
    }

    ScreenSetupDragWidget::~ScreenSetupDragWidget()
    {
    }

    QColor ScreenSetupDragWidget::color() const
    {
      return color_;
    }

    void ScreenSetupDragWidget::setColor(QColor const& _color)
    {
      color_=_color;
      update();
    }
 
    void ScreenSetupDragWidget::setRect(QRectF const& _rect)
    {
      setGeometry(_rect.toRect());
      raise();
    }
 
    void ScreenSetupDragWidget::paintEvent(QPaintEvent*)
    {
      QPainter _p(this);
      _p.setPen(Qt::NoPen);
      _p.setBrush(color());
      _p.drawRect(rect());
    }
  }
}

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

#include <omni/ui/BrushPreview.h>

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <omni/BlendBrush.h>

using namespace omni::ui;

BrushPreview::BrushPreview(QWidget* _parent) :
  QWidget(_parent),
  image_(new QImage(64,64,QImage::Format_RGB32))
{}

void BrushPreview::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  auto&& _buf = blend::Mask::generateBrush(64,feather_);
  using namespace gex::raster;
  for_each(_buf,strategy::for_each::Pixel(),[&](const uint8_t& _pix, size_t _x, size_t _y)
  {
    int _v = invert_ ? 255 -  _pix : _pix;
    image_->setPixel(_x,_y,QColor(_v,_v,_v).rgb());
  });
  painter.drawImage(0, 0, *image_);
  QWidget::paintEvent(event);
}


/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include <omni/BlendMask.h>

#include <omni/util.h>
#include <omni/proj/Tuning.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
  BlendMask::BlendMask(QObject* parent) :
    QObject(parent),
    rect_(0.1, 0.1, 0.8, 0.9)
  {
    // Stroke buffer size is fixed to 1 mega pixel
    strokeBuffer_.resize(resolution(), resolution());
  }

  void BlendMask::clear()
  {
    strokeBuffer_.clear();
  }

  float BlendMask::topWidth() const
  {
    return rect_.top();
  }

  float BlendMask::bottomWidth() const
  {
    return 1.0 - rect_.bottom();
  }

  float BlendMask::leftWidth() const
  {
    return rect_.left();
  }

  float BlendMask::rightWidth() const
  {
    return 1.0 - rect_.right();
  }

  void BlendMask::stamp(const QPointF& _pos)
  {
    brush_->stamp(transformedPoint(_pos), strokeBuffer_);
  }

  float BlendMask::drawLine(QPointF const& _p0,
                            QPointF const& _p1,
                            float          _leftOver)
  {
    return brush_->drawLine(
      transformedPoint(_p0),
      transformedPoint(_p1), strokeBuffer_, _leftOver);
  }

  QPointF BlendMask::transformedPoint(QPointF const& _pos) const {
    return float(resolution()) * QPointF(_pos.x(),_pos.y());
  }
}

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
  BlendMask::BlendMask(proj::Tuning const& _tuning) :
    tuning_(_tuning),
    rect_(0.1, 0.1, 0.8, 0.9),
    gamma_(2.0)
  {
    // Stroke buffer size is fixed to 1 mega pixel
    strokeBuffer_.resize(resolution(), resolution());
  }

  void BlendMask::clear()
  {
    strokeBuffer_.clear();
  }

  void BlendMask::setRect(QRectF const& _rect)
  {
    rect_ = _rect;
  }

  QRectF BlendMask::rect() const
  {
    return rect_;
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

  void BlendMask::setGamma(float _gamma)
  {
    gamma_ = _gamma;

    if (gamma_ < 0.1) gamma_ = 0.1;
  }

  float BlendMask::gamma() const
  {
    return gamma_;
  }

  void BlendMask::setBrush(
    float _size,
    float _feather, float _opacity, bool _invert) {
    _size = qBound(30.0f, _size, 512.0f);
    brush_.setBrush(QVector2D(transformedPoint(QPointF(_size,
                                                       _size))), _feather, _opacity,
                    _invert);
  }

  /// Invert brush
  void BlendMask::invertBrush(bool _invert) {
    brush_.setInvert(_invert);
  }

  /// Change brush size by +- amount of pixel
  void BlendMask::changeBrushSize(float _delta) {
    float _size = qBound(30.0f, brushSize() + _delta, 512.0f);

    brush_.setSize(QVector2D(transformedPoint(QPointF(_size, _size))));
  }

  BlendBrush const& BlendMask::brush() const
  {
    return brush_;
  }

  float BlendMask::brushSize() const {
    return brush_.size().x() * tuning_.width() / float(resolution());
  }

  void BlendMask::stamp(const QPointF& _pos)
  {
    brush_.stamp(transformedPoint(_pos), strokeBuffer_);
  }

  float BlendMask::drawLine(QPointF const& _p0,
                            QPointF const& _p1,
                            float          _leftOver)
  {
    return brush_.drawLine(
      transformedPoint(_p0),
      transformedPoint(_p1), strokeBuffer_, _leftOver);
  }

  BlendMask::buffer_type const& BlendMask::strokeBuffer() const
  {
    return strokeBuffer_;
  }

  void BlendMask::resize(int width, int height)
  {
    strokeBuffer_.resize(width, height);
  }

  void * BlendMask::strokeBufferData() const
  {
    return (void *)(strokeBuffer_.data().data());
  }

  QPointF BlendMask::transformedPoint(QPointF const& _pos) const {
    return float(resolution()) * QPointF(_pos.x() / tuning_.width(),
                                         _pos.y() / tuning_.height());
  }

  void BlendMask::toStream(QDataStream& _os) const {
    omni::PropertyMap _map;

    _map("rect", rect_)
      ("gamma", gamma_)
      ("brush", brush_)
      ("strokeBuffer", strokeBuffer_);
    _os << _map;
  }

  void BlendMask::fromStream(QDataStream& _is) {
    PropertyMap _map;

    _is >> _map;
    _map.get("rect", rect_);
    _map.get("gamma", gamma_);
    _map.get("brush",        brush_);
    _map.get("strokeBuffer", strokeBuffer_);
  }

  bool operator==(BlendMask const& _lhs, BlendMask const& _rhs)
  {
    return
      OMNI_TEST_MEMBER_EQUAL(rect_) &&
      OMNI_TEST_MEMBER_EQUAL(gamma_) &&
      OMNI_TEST_MEMBER_EQUAL(brush_) &&
      OMNI_TEST_MEMBER_EQUAL(strokeBuffer_);
  }
}

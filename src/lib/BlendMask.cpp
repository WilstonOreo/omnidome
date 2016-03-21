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
        autoResizeStrokeBuffer();
    }

    void BlendMask::clear()
    {
        strokeBuffer_.clear();
    }

    void BlendMask::setRect(QRectF const& _rect, bool _update)
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

    BlendBrush& BlendMask::brush()
    {
        return brush_;
    }

    BlendBrush const& BlendMask::brush() const
    {
        return brush_;
    }

    void BlendMask::stamp(const QPointF& _pos)
    {
        autoResizeStrokeBuffer();
        brush_.stamp(_pos, strokeBuffer_);
    }

    float BlendMask::drawLine(QPointF const& _p0,
                              QPointF const& _p1,
                              float          _leftOver)
    {
        autoResizeStrokeBuffer();
        return brush_.drawLine(_p0, _p1, strokeBuffer_, _leftOver);
    }

    BlendMask::Buffer const& BlendMask::strokeBuffer() const
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

    void BlendMask::autoResizeStrokeBuffer()
    {
        if ((strokeBuffer_.width() != tuning_.width()) ||
            (strokeBuffer_.height() != tuning_.height())) strokeBuffer_.resize(
                tuning_.width(),
                tuning_.height());
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

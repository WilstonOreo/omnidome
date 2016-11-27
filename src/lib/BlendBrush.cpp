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

#include <omni/BlendBrush.h>

#include <omni/util.h>
#include <QPointF>
#include <QVector2D>
#include <QDataStream>
#include <omni/serialization/PropertyMap.h>

namespace omni {
    BlendBrush::BlendBrush() : size_(100.0,100.0)
    {
        generate();
    }

    BlendBrush::BlendBrush(QVector2D const& _size, float _feather) :
        size_(_size),
        feather_(_feather)
    {
        generate();
    }

    QVector2D const& BlendBrush::size() const
    {
        return size_;
    }

    void BlendBrush::setSize(QVector2D const& _size)
    {
        size_ = clampBrushSize(_size);
        generate();
    }

    void BlendBrush::changeSize(QVector2D const& _delta)
    {
        setSize(size_ + _delta);
    }

    float BlendBrush::opacity() const {
        return opacity_;
    }

    void BlendBrush::setOpacity(float _opacity) {
        opacity_ = _opacity;

        if (opacity_ < 0.0) {
            opacity_ = 0.0;
        }

        if (opacity_ > 1.0) {
            opacity_ = 1.0;
        }
        generate();
    }

    float BlendBrush::feather() const
    {
        return feather_;
    }

    void BlendBrush::setFeather(float _feather)
    {
        feather_ = _feather;

        if (feather_ < 0.0) feather_ = 0.0;

        if (feather_ > 10.0) feather_ = 10.0;
        generate();
    }

    bool BlendBrush::invert() const
    {
        return invert_;
    }

    void BlendBrush::setInvert(bool _invert)
    {
        invert_ = _invert;
        generate();
    }

    void BlendBrush::setBrush(
        QVector2D const& _size,
        float _feather,
        float _opacity, bool _invert)
    {
        size_ = clampBrushSize(_size);

        feather_ = _feather;

        if (feather_ < 0.0) feather_ = 0.0;
        if (feather_ > 10.0) feather_ = 10.0;
        invert_ = _invert;

        if (opacity_ < 0.0) {
            opacity_ = 0.0;
        }

        if (opacity_ > 1.0) {
            opacity_ = 1.0;
        }
        opacity_ = _opacity;
        generate();
    }

    void BlendBrush::stamp(QPointF const& _p, Buffer<uint8_t>& _buf) const
    {
        int _sizeX = size().x();
        int _sizeY = size().y();
        QVector2D r     = size() * 0.5;

        int dx = _p.x() - r.x();
        int dy = _p.y() - r.y();

        for (int i = 0; i < _sizeX; ++i)
        {
            int _posx = int(i + dx);

            if ((_posx < 0) || (_posx >= _buf.width())) continue;

            for (int j = 0; j < _sizeY; ++j)
            {
                int _posy = int(j + dy);

                if ((_posy < 0) || (_posy >= _buf.height())) continue;

                auto  _v   = buffer_(i, j);
                auto& _pix = _buf(_posx, _posy);

                if (invert())
                {
                    _pix = _pix * _v;
                }
                else
                {
                    _pix = (255 - (255 - _buf(_posx, _posy)) * _v);
                }
            }
        }
    }

    float BlendBrush::drawLine(const QPointF& _p0, const QPointF& _p1,
                               Buffer<uint8_t>& _buf, float _leftOver)
    {
        float _spacing = size_.length() / 10.0;
        if (_spacing < 0.5) _spacing = 0.5;

        QVector2D _step(0.0, 0.0);

        // Calculate vector and distance
        QVector2D _delta(_p1 - _p0);
        float     _dist = _delta.length();

        float _invDist = 0.0;

        if (_dist > 0.0)
        {
            _invDist = 1.0 / _dist;
            _step   += _delta * _invDist;
        }

        // Draw line
        QVector2D _offset(0.0, 0.0);
        float     _totalDistance = _leftOver + _dist;

        while (_totalDistance >= _spacing)
        {
            if (_leftOver > 0)
            {
                _offset   += _step * (_spacing - _leftOver);
                _leftOver -= _spacing;
            }
            else
            {
                _offset += _step * _spacing;
            }

            // Draw stamp
            stamp(_p0 + _offset.toPointF(), _buf);

            _totalDistance -= _spacing;
        }

        return _totalDistance;
    }

    Buffer<float>const& BlendBrush::buffer() const
    {
        return buffer_;
    }

    void BlendBrush::generate()
    {
        int _sizeX = std::max(int(size().x() + 1), 2);
        int _sizeY = std::max(int(size().y() + 1), 2);
        buffer_.resize(_sizeX, _sizeY);

        float _r = 0.49;
        float _innerRadius = _r - _r * feather();


        // For each pixel
        for (int y = 0; y < _sizeY; ++y)
            for (int x = 0; x < _sizeX; ++x)
            {
              QVector2D _pos(float(x) / _sizeX - 0.5, float(y) / _sizeY - 0.5);

                float _distance = _pos.length();

                // Pixel value
                float _v = ((_distance - _innerRadius) / (_r - _innerRadius)) *
                           opacity_ + 1.0 - opacity_;

                // Clamp and set pixel value
                buffer_(x, y) = qBound(0.0f, _v, 1.0f);
            }
    }

    QVector2D BlendBrush::clampBrushSize(QVector2D const& _size) {
        float _aspect = _size.x() / _size.y();
        return QVector2D(
          qBound(30.0f,_size.x(),  400.0f),
          qBound(30.0f / _aspect,_size.y(),  400.0f / _aspect));
    }

    /// Write blend brush to stream
    void BlendBrush::toStream(QDataStream& _os) const {
        PropertyMap _map;

        _map("size", size_)
            ("feather", feather_)
            ("opacity", opacity_)
            ("invert", invert_);
        _os << _map;
    }

    /// Read blend brush from stream
    void BlendBrush::fromStream(QDataStream& _is) {
        PropertyMap _map;
        _is >> _map;
        _map.get("size",    size_);
        _map.get("feather", feather_);
        _map.get("opacity", opacity_);
        _map.get("invert",  invert_);
        generate();
    }

    bool operator==(BlendBrush const& _lhs, BlendBrush const& _rhs)
    {
        return
            OMNI_TEST_MEMBER_EQUAL(size_) &&
            OMNI_TEST_MEMBER_EQUAL(feather_) &&
            OMNI_TEST_MEMBER_EQUAL(opacity_) &&
            OMNI_TEST_MEMBER_EQUAL(invert_);
    }
}

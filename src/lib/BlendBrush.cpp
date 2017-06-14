/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
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
    BlendBrush::BlendBrush(QObject* parent) :
      QObject(parent),
      size_(100.0,100.0)
    {
        connect(this,&BlendBrush::sizeChanged,this,&BlendBrush::generate);
        connect(this,&BlendBrush::featherChanged,this,&BlendBrush::generate);
        connect(this,&BlendBrush::invertedChanged,this,&BlendBrush::generate);
        connect(this,&BlendBrush::opacityChanged,this,&BlendBrush::generate);
        generate();
    }

    QVector2D const& BlendBrush::size() const
    {
        return size_;
    }

    void BlendBrush::setSize(QVector2D const& _size)
    {
      if (size_ != _size) {
        size_ = clampBrushSize(_size);
        emit sizeChanged();
      }
    }

    void BlendBrush::changeSize(QVector2D const& _delta)
    {
        setSize(size_ + _delta);
    }

    OMNI_PROPERTY_RW_IMPL(BlendBrush,bool,inverted,setInverted)
    OMNI_PROPERTY_REAL_BOUND_IMPL(BlendBrush,feather,setFeather,0.0,10.0)
    OMNI_PROPERTY_REAL_BOUND_IMPL(BlendBrush,opacity,setOpacity,0.0,1.0)

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

                if (inverted())
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
}

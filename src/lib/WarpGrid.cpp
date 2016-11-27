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

#include <omni/WarpGrid.h>

#include <QDataStream>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/serialization/container.h>

namespace omni {
    WarpGrid::WarpGrid() :
        horizontal_(4),
        vertical_(4)
    {
        reset();
    }

    QVector2D WarpGrid::getTexCoord(int _x, int _y) const
    {
        return QVector2D(float(_x) / (horizontal() - 1),
                         float(_y) / (vertical() - 1));
    }

    void WarpGrid::reset()
    {
        points_.clear();
        points_.reserve(vertical() * horizontal());

        for (size_t y = 0; y < vertical(); ++y)
            for (size_t x = 0; x < horizontal();
                 ++x) points_.emplace_back((getTexCoord(x, y) - QVector2D(0.5,
                                                                          0.5)).toPointF());
        hasChanged_ = true;
    }

    void WarpGrid::resize(int _horz, int _vert)
    {
        if ((_horz < 2) || (_vert < 2) ||
            ((_horz == horizontal_) && (_vert == vertical_))) return;

        horizontal_ = _horz;
        vertical_   = _vert;
        reset();
    }

    int WarpGrid::vertical() const
    {
        return vertical_;
    }

    int WarpGrid::horizontal() const
    {
        return horizontal_;
    }

    /// Return interpolation type (BICUBIC is default)
    WarpGrid::Interpolation WarpGrid::interpolation() const {
        return interpolation_;
    }

    /// Interpolation value
    void WarpGrid::setInterpolation(Interpolation _interpolation) {
        interpolation_ = _interpolation;
        hasChanged_ = true;
    }

    void WarpGrid::selectAll()
    {
        for (auto& _point : points_) _point.setSelected(true);
    }

    WarpPoint * WarpGrid::selectNearest(const QPointF& _p)
    {
        size_t _nearestIdx = getNearest(_p);
        auto   _nearest    =
            (_nearestIdx == -1) ? nullptr : &points_[_nearestIdx];
        hasChanged_ = true;

        return _nearest;
    }

    void WarpGrid::selectNone()
    {
        for (auto& _point : points_) _point.setSelected(false);
    }

    QVector2D WarpGrid::getWarpPointPos(int x, int y) const
    {
        int maxX = horizontal() - 1;
        int maxY = vertical() - 1;

        // here's the magic: extrapolate points beyond the edges
        if (x < 0) {
            return QVector2D(
                2.0f * getWarpPointPos(0, y) - getWarpPointPos(0 - x, y));
        }

        if (y < 0) {
            return QVector2D(
                2.0f * getWarpPointPos(x, 0) - getWarpPointPos(x, 0 - y));
        }

        if (x > maxX) {
            return QVector2D(2.0f * getWarpPointPos(maxX, y) - getWarpPointPos(
                                 2 * maxX - x,
                                 y));
        }

        if (y > maxY) {
            return QVector2D(
                2.0f * getWarpPointPos(x, maxY)
                - getWarpPointPos(x, 2 * maxY - y));
        }


        return QVector2D(getPoint(x, y)->pos());
    }

    // from http://www.paulinternet.nl/?page=bicubic : fast catmull-rom
    // calculation
    QVector2D WarpGrid::cubicInterpolate(
        const array4_type& knots,
        float t) const
    {
        return knots[1] + 0.5f * t * (knots[2] - knots[0] +
                                      t * (2.0f * knots[0] - 5.0f * knots[1] +
                                           4.0f * knots[2] - knots[3] +
                                           t * (3.0f * (knots[1] - knots[2]) +
                                                knots[3] - knots[0])));
    }

    QVector2D WarpGrid::getWarpPointPos(int x, int y, float u, float v) const {
        switch (interpolation_) {
        default:

        // perform linear interpolation
        case Interpolation::LINEAR:
        {
            auto _p00 = getWarpPointPos(x + 0, y + 0);
            auto _p10 = getWarpPointPos(x + 1, y + 0);
            auto _p01 = getWarpPointPos(x + 0, y + 1);
            auto _p11 = getWarpPointPos(x + 1, y + 1);
            QVector2D p1((1.0f - u) * _p00 + u * _p10);
            QVector2D p2((1.0f - u) * _p01 + u * _p11);
            return QVector2D((1.0f - v) * p1 + v * p2);
        }
        // perform bicubic interpolation
        case Interpolation::BICUBIC:
        {
            array4_type _rows, _cols;
            for (int i = -1; i < 3; ++i) {
                for (int j = -1; j < 3; ++j) {
                    _cols[j + 1] = getWarpPointPos(x + i, y + j);
                }
                _rows[i + 1] = cubicInterpolate(_cols, v);
            }
            return cubicInterpolate(_rows, u);
        }
        }
        return QVector2D(0.0, 0.0);
    }

    std::set<WarpPoint *>WarpGrid::getSelected()
    {
        std::set<WarpPoint *> _selection;

        hasChanged_ = true;

        for (auto& _point : points_)
            if (_point.selected()) _selection.insert(&_point);
        return _selection;
    }

    std::set<WarpPoint const *>WarpGrid::getSelected() const
    {
        std::set<WarpPoint const *> _selection;

        for (auto& _point : points_)
            if (_point.selected()) _selection.insert(&_point);
        return _selection;
    }

    WarpPoint * WarpGrid::getPoint(int x, int y)
    {
        if ((x < 0) || (y < 0)) return nullptr;

        if ((x >= horizontal_) || (y >= vertical_)) return nullptr;
        hasChanged_ = true;

        return &points_[y * horizontal_ + x];
    }

    WarpPoint const * WarpGrid::getPoint(int x, int y) const
    {
        if ((x < 0) || (y < 0)) return nullptr;

        if ((x >= horizontal_) || (y >= vertical_)) return nullptr;

        return &points_[y * horizontal_ + x];
    }

    size_t WarpGrid::getNearest(const QPointF& _p) const
    {
        float  _nearestDist = std::numeric_limits<float>::max();
        size_t _nearestIdx  = -1;

        for (size_t i = 0; i < points_.size(); ++i)
        {
            auto& _point = points_[i];
            auto  _dist  = QVector2D(_p - _point.pos()).lengthSquared();

            if (_dist < _nearestDist)
            {
                _nearestIdx  = i;
                _nearestDist = _dist;
            }
        }
        return _nearestIdx;
    }

    bool WarpGrid::isReset() const
    {
        for (size_t y = 0; y < vertical(); ++y)
            for (size_t x = 0; x < horizontal(); ++x)
            {
                if (QVector2D(getPoint(x,
                                       y)->pos()) !=
                    (getTexCoord(x, y) - QVector2D(0.5, 0.5))) return false;
            }

        return true;
    }

    bool WarpGrid::hasChanged() const {
        return hasChanged_;
    }

    std::vector<WarpPoint>const& WarpGrid::points() const
    {
        return points_;
    }

    bool operator==(WarpGrid const& _lhs, WarpGrid const& _rhs)
    {
        return
            OMNI_TEST_MEMBER_EQUAL(horizontal_) &&
            OMNI_TEST_MEMBER_EQUAL(vertical_) &&
            OMNI_TEST_MEMBER_EQUAL(points_);
    }
}

QDataStream& operator<<(QDataStream& _os, const omni::WarpGrid& _p)
{
    omni::serialization::PropertyMap _map;
    _map
        ("horizontal",_p.horizontal())
        ("vertical",_p.vertical())
        ("interpolation",omni::util::enumToInt(_p.interpolation()));
    _os << _map;
    for (auto& _point : _p.points()) _os << _point;

    return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::WarpGrid& _p)
{
    using namespace omni;

    omni::serialization::PropertyMap _map;
    _is >> _map;
    int _horizontal = _map.getValue<int>("horizontal");
    int _vertical = _map.getValue<int>("vertical");
    int _interpolation = _map.getValue<int>("interpolation",
        util::enumToInt(WarpGrid::Interpolation::BICUBIC));

    _p.resize(_horizontal, _vertical);
    _p.setInterpolation(util::intToEnum<WarpGrid::Interpolation>(_interpolation));

    for (int y = 0; y < _p.vertical(); ++y)
        for (int x = 0; x < _p.horizontal(); ++x) _is >> *_p.getPoint(x, y);

    return _is;
}

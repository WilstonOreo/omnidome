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

#include <omni/WarpGrid.h>

#include <iostream>
#include <QDataStream>
#include <omni/util.h>

namespace omni {
    WarpGrid::WarpGrid() :
        horizontal_(4),
        vertical_(4)
    {
        resize(horizontal_, vertical_);
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

    }

    void WarpGrid::resize(int _horz, int _vert)
    {
        if ((_horz < 2) || (_vert < 2)) return;

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

    void WarpGrid::selectAll()
    {
        for (auto& _point : points_) _point.setSelected(true);
    }

    WarpPoint * WarpGrid::selectNearest(const QPointF& _p)
    {
        size_t _nearestIdx = getNearest(_p);
        auto   _nearest    =
            (_nearestIdx == -1) ? nullptr : &points_[_nearestIdx];

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
                2.0f * getWarpPointPos(0, y) - getWarpPointPos(0 - x,y));
        }

        if (y < 0) {
            return QVector2D(
                2.0f * getWarpPointPos(x, 0) - getWarpPointPos(x,0 - y));
        }

        if (x > maxX) {
            return QVector2D(2.0f * getWarpPointPos(maxX, y) - getWarpPointPos(
                2 * maxX - x,
                y));
        }

        if (y > maxY) {
            return QVector2D(
                2.0f * getWarpPointPos(x, maxY)
                - getWarpPointPos(x,2 * maxY -y));
        }


        return QVector2D(getPoint(x,y)->pos());
    }

    std::set<WarpPoint *>WarpGrid::getSelected()
    {
        std::set<WarpPoint *> _selection;

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
    _os << _p.horizontal() << _p.vertical();

    for (auto& _point : _p.points()) _os << _point;
    return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::WarpGrid& _p)
{
    int _horizontal, _vertical;

    _is >> _horizontal >> _vertical;
    _p.resize(_horizontal, _vertical);

    for (int y = 0; y < _p.vertical(); ++y)
        for (int x = 0; x < _p.horizontal(); ++x) _is >> *_p.getPoint(x, y);

    return _is;
}

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

#include <omni/WarpPoint.h>

#include <omni/util.h>
#include <QDataStream>

namespace omni
{
  WarpPoint::WarpPoint()
  {
  }

  WarpPoint::WarpPoint(
      QPointF const& _pos,
      QPointF const& _c1,
      QPointF const& _c2) :
    pos_(_pos),
    c1_(_c1),
    c2_(_c2),
    selected_(false)
  {
  }

  void WarpPoint::setPos(QPointF const& _pos)
  {
    pos_=_pos;
  }

  QPointF& WarpPoint::pos()
  {
    return pos_;
  }

  QPointF const& WarpPoint::pos() const
  {
    return pos_;
  }

  float WarpPoint::x() const
  {
    return pos_.x();
  }
  
  float WarpPoint::y() const
  {
    return pos_.y();
  }

  void WarpPoint::setC1(QPointF const& _c1)
  {
    c1_=_c1;
  }

  QPointF& WarpPoint::c1()
  {
    return c1_;
  }

  QPointF const& WarpPoint::c1() const
  {
    return c1_;
  }
  
  void WarpPoint::setC2(QPointF const& _c2)
  {
    c2_=_c2;
  }

  QPointF& WarpPoint::c2()
  {
    return c2_;
  }

  QPointF const& WarpPoint::c2() const
  {
    return c2_;
  }

  void WarpPoint::setSelected(bool _selected)
  {
    selected_=_selected;
  }

  bool WarpPoint::selected() const
  {
    return selected_;
  }
  
  bool operator==(WarpPoint const& _lhs, WarpPoint const& _rhs)
  {
    return 
      OMNI_TEST_MEMBER_EQUAL(pos_) &&
      OMNI_TEST_MEMBER_EQUAL(c1_) &&
      OMNI_TEST_MEMBER_EQUAL(c2_) &&
      OMNI_TEST_MEMBER_EQUAL(selected_);
  }
}

QDataStream& operator<<(QDataStream& _os, const omni::WarpPoint& _p)
{
  _os << _p.pos() << _p.c1() << _p.c2() << _p.selected();
  return _os;
}

QDataStream& operator>>(QDataStream& _is, omni::WarpPoint& _p)
{
  _is >> _p.pos();
  _is >> _p.c1();
  _is >> _p.c2();

  bool _selected = false;
  _is >> _selected;
  _p.setSelected(_selected);
  return _is;
}



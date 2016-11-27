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

#include <omni/Id.h>

#include <QString>
#include <QDebug>
#include <omni/serialization/traits.h>

namespace omni
{
  Id::Id()
  {
  }

  Id::Id(QString const& _str)
  {
    make(_str);
  }

  Id::Id(const char* _c)
  {
    make(QString(_c));
  }

  QString const& Id::str() const
  {
    return str_;
  }

  Id::operator QString() const
  {
    return str_;
  }

  bool Id::valid() const
  {
    return !str_.isEmpty();
  }

  bool Id::operator<(const Id& _rhs) const
  {
    return this->str_ < _rhs.str_;
  }

  bool Id::operator!=(const Id& _rhs) const
  {
    return !(*this == _rhs);
  }

  bool Id::operator==(const Id& _rhs) const
  {
    return this->str_ == _rhs.str_;
  }

  void Id::make(QString const& _str)
  {
    str_ = "";
    // An id must not be empty
    if (_str.isEmpty()) return;

    // An Id must begin with a letter
    if (!_str[0].isLetter()) return;

    // An id must only contain alpha numerical characters
    // and underscores
    for (auto& c : _str)
    {
      if (!c.isLetterOrNumber() && (!c.isPunct())) return;
    }
    str_=_str;
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::Id const& _id)
{
  omni::serialization::serialize(_stream,_id.str());
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::Id& _id)
{
  QString _str;
  omni::serialization::deserialize(_stream,_str);
  _id = omni::Id(_str);
  return _stream;
}

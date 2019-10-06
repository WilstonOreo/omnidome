/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#include <omni/LengthUnit.h>

#include <omni/util.h>

namespace omni {
  LengthUnit::LengthUnit(Type _type) :
    type_(_type)
  {}

  LengthUnit::LengthUnit(QString const& _abbr)
  {
    type_ = type(_abbr);
  }

  QString LengthUnit::abbreviation(Type _type) {
    static map_type abbrMap_;

    if (abbrMap_.empty()) {
      abbrMap_ = map_type({
        { METER, "m" },
        { MILLIMETER, "mm" },
        { CENTIMETER, "cm" },
        { INCH, "in" },
        { FOOT, "ft" }
      });
    }

    if (abbrMap_.count(_type) == 0) {
      return "INVALID_UNIT";
    }
    return abbrMap_.at(_type);
  }

  QString LengthUnit::abbreviation() const {
    return abbreviation(type_);
  }

  /// Get unit type from abbreviation
  LengthUnit::Type LengthUnit::type(QString const& abbr) {
    static std::map<QString, Type> typeMap_;

    if (typeMap_.empty()) {
      typeMap_ = std::map<QString, Type>({
        { "m", METER },
        { "mm", MILLIMETER },
        { "cm", CENTIMETER },
        { "in", INCH },
        { "ft", FOOT }
      });
    }

    if (typeMap_.count(abbr) == 0) {
      return INVALID_UNIT;
    }
    return typeMap_.at(abbr);
  }

  /// Return type of unit
  LengthUnit::Type LengthUnit::type() const {
    return type_;
  }

  /// Set type of unit
  void LengthUnit::setType(Type _type) {
    type_ = _type;
  }

  /// Return singular name of unit with type
  QString LengthUnit::nameSingular(Type _type) {
    static map_type nsMap_;

    if (nsMap_.empty()) {
      nsMap_ = map_type({
        { METER, "meter" },
        { MILLIMETER, "millimeter" },
        { CENTIMETER, "centimeter" },
        { INCH, "inch" },
        { FOOT, "foot" }
      });
    }

    if (nsMap_.count(_type) == 0) {
      return "INVALID_UNIT";
    }
    return nsMap_.at(_type);
  }

  /// Return singular name of unit
  QString LengthUnit::nameSingular() const {
    return nameSingular(type_);
  }

  /// Return plural name of unit with type
  QString LengthUnit::namePlural(Type _type) {
    static map_type npMap_;

    if (npMap_.empty()) {
      npMap_ = map_type({
        { METER, "meters" },
        { MILLIMETER, "millimeters" },
        { CENTIMETER, "centimeters" },
        { INCH, "inches" },
        { FOOT, "feet" }
      });
    }

    if (npMap_.count(_type) == 0) {
      return "INVALID_UNIT";
    }
    return npMap_.at(_type);
  }

  /// Return plural name of unit
  QString LengthUnit::namePlural() const {
    return namePlural(type_);
  }

  /// Deserialize from stream
  void LengthUnit::fromStream(QDataStream& _is) {
    _is >> type_;
  }

  /// Serialize to stream
  void LengthUnit::toStream(QDataStream& _os) const {
    _os << type_;
  }

  /// Test for equality. ScreenSetup is ignored
  bool operator==(LengthUnit const& _lhs, LengthUnit const& _rhs) {
    return OMNI_TEST_MEMBER_EQUAL(type_);
  }
}

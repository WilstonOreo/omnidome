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
#ifndef OMNI_LENGTHUNIT_H_
#define OMNI_LENGTHUNIT_H_

#include <map>
#include <QString>
#include <QDataStream>
#include <omni/serialization/Interface.h>

namespace omni {
  /// An encapsulation for length units
  class OMNI_EXPORT LengthUnit {
    public:
      /// Supported unit types
      enum Type {
        METER, MILLIMETER, CENTIMETER,
        INCH, FOOT, NUM_UNIT_TYPES,
        INVALID_UNIT = -1
      };

      /// Make a new unit with a certain type
      LengthUnit(Type = METER);

      /// Make a new unit from abbreviation type
      LengthUnit(QString const&);

      /// Return abbreviation for unit with type
      static QString abbreviation(Type);

      /// Return abbreviation of unit
      QString     abbreviation() const;

      /// Get unit type from abbreviation
      static Type type(QString const& abbr);

      /// Return type of unit
      Type        type() const;

      /// Set type of unit
      void setType(Type);

      /// Return singular name of unit with type
      static QString nameSingular(Type);

      /// Return singular name of unit
      QString nameSingular() const;

      /// Return plural name of unit with type
      static QString namePlural(Type);

      /// Return plural name of unit
      QString namePlural() const;

      /// Deserialize from stream
      void        fromStream(QDataStream&);

      /// Serialize to stream
      void        toStream(QDataStream&) const;

      /// Test for equality. ScreenSetup is ignored
      friend bool operator==(LengthUnit const&,
                             LengthUnit const&);

    private:
      typedef std::map<Type, QString>map_type;

      Type type_;
  };
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::LengthUnit::Type)
OMNI_DECL_STREAM_OPERATORS(omni::LengthUnit)

#endif /* OMNI_LENGTHUNIT_H_ */

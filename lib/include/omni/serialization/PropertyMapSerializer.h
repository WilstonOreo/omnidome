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
#ifndef OMNI_SERIALIZATION_PROPERTYMAPSERIALIZER_H_
#define OMNI_SERIALIZATION_PROPERTYMAPSERIALIZER_H_

#include <omni/serialization/Interface.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
  namespace serialization {
    /// Interface for serializing data into a property map
    class PropertyMapSerializer : public serialization::Interface {
    public:
      virtual void fromPropertyMap(PropertyMap const& _map) = 0;
      virtual void toPropertyMap(PropertyMap& _map) const = 0;

      inline void fromStream(QDataStream& _is) {
        PropertyMap _map;
        _is >> _map;
        fromPropertyMap(_map);
      }

      inline void toStream(QDataStream& _os) const {
        PropertyMap _map;
        toPropertyMap(_map);
        _os << _map;
      }
    };
  }

  using serialization::PropertyMapSerializer;
}

#endif /* OMNI_SERIALIZATION_PROPERTYMAPSERIALIZER_H_ */

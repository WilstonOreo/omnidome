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
#ifndef OMNI_SERIALIZATION_SERIALIZER_H_
#define OMNI_SERIALIZATION_SERIALIZER_H_

namespace omni {
  namespace serialization {
    template<typename T>
    struct Serializer {

      Serializer() {
        static_assert(std::is_base_of<QObject,T>::value,
          "T must be a base of QObject!");
      }

      QVariant toVariant() const {
        auto mo = metaObject();
        QVariantMap map;
        for (int i = 0; i < mo->propertyCount(); ++i) {
          auto prop = mo->property(i);
          if (!prop.stored()) continue;
          map[prop.name()] = prop.read(this);
        }
        return QVariant::fromValue(map);
      }

      void fromVariant(QVariant const& v) {
        if (!v.canConvert<QVariant>()) return;
        auto mo = metaObject();
        auto map = v.toMap();
        for (int i = 0; i < mo->propertyCount(); ++i) {
          auto prop = mo->property(i);
          if (!prop.isStored()) continue;
          prop.write(this,map[prop.name()]);
        }
      }

      bool equal(T const& other) const {
        auto* thisMo = metaObject();
        auto* thatMo = other.metaObject();
        if (thisMo->propertyCount() != thatMo->propertyCount()) return false;

        int count = this->propertyCount();
        for (int i = 0; i < count; ++i) {
            auto thisProp = thisMo->property(i);
            auto thatProp = thatMo->property(i);
            if (!thisProp.isStored() || !thatProp.isStored()) continue;

            QVariant thisVal = thisProp.read(this);
            QVariant thatVal = thatProp.read(&other);
            if (thisVal != thatVal) return false;
        }
        return true;
      }

    private:
      QMetaObject* metaObject() const {
        return this->metaObject();
      }
    };
  }

  using serialization::Serializer;
}

#endif /* OMNI_SERIALIZATION_SERIALIZER_H_ */

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

#include <QObject>
#include <QMetaProperty>
#include <QVariant>
#include <iostream>

namespace omni {
  namespace serialization {
    template<typename T>
    struct Serializer {

      Serializer() {
        static_assert(std::is_base_of<QObject,T>::value,
          "T must be a base of QObject!");
      }

      QVariant toVariant() const {
        return toVariant(static_cast<T const*>(this));
      }

      void fromVariant(QVariant const& v) {
        fromVariant(static_cast<T*>(this),v);
      }

      bool equal(T const* other) const {
        auto* thisMo = metaObject();
        auto* thatMo = other->metaObject();
        if (thisMo->propertyCount() != thatMo->propertyCount()) return false;

        return this->toVariant() == other->toVariant();
      }

      void copy(T* other) {
          other->fromVariant(this->toVariant());
      }

    private:
      QMetaObject const* metaObject() const {
        return static_cast<T const*>(this)->metaObject();
      }

      static QVariant toVariant(QObject const* o) {
        auto mo = o->metaObject();
        QVariantMap map;
        for (int i = 0; i < mo->propertyCount(); ++i) {
          auto prop = mo->property(i);
          if (!prop.isStored()) continue;

          if (!prop.isWritable() && prop.isConstant()) {
              auto subO = qvariant_cast<QObject*>(prop.read(o));
              if (!subO) continue;
              map[prop.name()] = toVariant(subO);
              continue;
          }
          map[prop.name()] = prop.read(o);
        }
        return map;
      }

      static void fromVariant(QObject* o, QVariant const& v) {
        if (!v.canConvert<QVariantMap>()) return;
        auto mo = o->metaObject();
        auto map = v.toMap();
        for (int i = 0; i < mo->propertyCount(); ++i) {
          auto prop = mo->property(i);
          if (!prop.isStored()) continue;
          if (!prop.isWritable() && prop.isConstant()) {
              qDebug() << prop.name();
              auto subO = qvariant_cast<QObject*>(prop.read(o));
              if (!subO) continue;
              fromVariant(subO,map[prop.name()]);
              continue;
          }
          prop.write(o,map[prop.name()]);
        }
      }


    };
  }

  using serialization::Serializer;
}

#endif /* OMNI_SERIALIZATION_SERIALIZER_H_ */

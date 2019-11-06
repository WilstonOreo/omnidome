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
#ifndef OMNI_SERIALIZATION_PROPERTYMAP_H_
#define OMNI_SERIALIZATION_PROPERTYMAP_H_

#include <memory>
#include <map>
#include <functional>
#include <QString>
#include <QByteArray>
#include <QBuffer>
#include <QDataStream>
#include <omni/Factory.h>
#include <omni/exception.h>
#include <omni/serialization/traits.h>
#include <omni/serialization/pointer.h>
#include <omni/serialization/Interface.h>

namespace omni {
  namespace serialization {
    namespace exception {
      /// Checksum exception which is raids when checksums mismatch
      class OMNI_EXPORT ChecksumError : public Error {
        public:
          OMNI_EXCEPTION(ChecksumError)

          ChecksumError(QString _got, QString _expected, QString const& _id = QString()) :
            got_(_got),
            expected_(_expected),
            id_(_id) {}

          inline QString message() const throw() {
            QString _s("ChecksumError: Expected %1, got %2.");

            _s = _s.arg(expected_).arg(got_);

            if (!id_.isEmpty()) {
              _s += QString(" On id %1.").arg(QString(id_));
            }
            return _s;
          }

        private:
          QString got_;
          QString expected_;
          QString id_;
      };

      class OMNI_EXPORT PropertyExisting : public Error {
        public:
          OMNI_EXCEPTION(PropertyExisting)

          PropertyExisting(QString const& _id) : id_(_id) {}

          inline QString message() const throw() {
            return QString("Property with id %1 already exists!").arg(QString(id_));
          }

        private:
          QString id_;
      };

      class OMNI_EXPORT PropertyNotExisting : public Error {
        public:
          OMNI_EXCEPTION(PropertyNotExisting)

          PropertyNotExisting(QString const& _id) : id_(_id) {}

          inline QString message() const throw() {
            return QString("Property with id %1 does not exist!").arg(id_);
          }

        private:
          QString id_;
      };
    }

    /**@brief Property map to store properties in a QBuffer with an id
    **/
    class OMNI_EXPORT PropertyMap : public Interface {
      public:
        PropertyMap();

        /// Construct from stream
        PropertyMap(QDataStream&);

        /// Put an object of type T with id into property map
        template<typename T, typename ... ARGS>
        PropertyMap& put(QString const& _id, T const& _t, ARGS&& ... _args) {
          if (properties_.count(_id) != 0) {
            throw exception::PropertyExisting(_id);
          }
          QBuffer _buf;
          _buf.open(QIODevice::WriteOnly);
          QDataStream _s(&_buf); 
          serialize(_s, _t, _args ...);
          properties_[_id] = _buf.data();
          return *this;
        }

        /// Read an object of type T with id from property map
        template<typename T, typename ... ARGS>
        PropertyMap const& get(QString const& _id, T& _t, ARGS&& ... _args) const {
          if (properties_.count(_id) == 0) {
            throw exception::PropertyNotExisting(_id);
            return *this;
          }

          QDataStream _s(properties_.at(_id));
          deserialize(_s, _t, _args ...);
          return *this;
        }

        /// Read an object of type T with id from property map
        template<typename T, typename ... ARGS>
        PropertyMap const& get(QString const& _id, T *_t, ARGS&& ... _args) const {
          if (properties_.count(_id) == 0) {
            throw exception::PropertyNotExisting(_id);
            return *this;
          }

          QDataStream _s(properties_.at(_id));
          deserialize(_s, _t, _args ...);
          return *this;
        }

        /// Get value from id (with optional default value when id is not present)
        template<typename T>
        T getValue(QString const& _id, T const& _default = T()) const {
          if (properties_.count(_id) == 0) {
            return _default;
          }
          T _t;
          QDataStream _s(properties_.at(_id));
          deserialize(_s, _t);
          return _t;
        }

        /// Read an object of type T with id from property map
        template<typename FACTORY_FUNCTOR, typename ... ARGS>
        PropertyMap const& getPtr(QString const& _id,
                                  FACTORY_FUNCTOR _f,
                                  ARGS&& ... _args) const {
          if (properties_.count(_id) == 0) {
            throw exception::PropertyNotExisting(_id);
          }
          QDataStream _s(properties_.at(_id));
          deserializePtr(_s, _f, _args ...);
          return *this;
        }

        /**@brief Get value from
        **/
        template<typename T, typename CLASS, typename MEM_FN>
        PropertyMap const& get(QString const& _id,
                               CLASS& _class,
                               MEM_FN _memFn,
                               T const& _default = T()) const {
          T _value = _default;

          try {
            get<T>(_id, _value);
            _memFn(_class, _value);
          } catch (exception::PropertyNotExisting& e) {
            return *this;
          }
          return *this;
        }

        /// Write an object into property map
        template<typename T, typename ... ARGS>
        PropertyMap& operator()(QString const& _id, T const& _t) {
          return put(_id, _t);
        }

        /// Read an object from property map
        template<typename T>
        PropertyMap const& operator()(QString const& _id, T& _t) const {
          return get(_id, _t);
        }

        /// Return list of ids
        std::vector<QString> ids() const {
          std::vector<QString> _ids;
          for (auto& _idValue : properties_) {
            _ids.push_back(_idValue.first);
          }
          return _ids;
        }

        QByteArray const& data(QString const& id) const {
            return properties_.at(id);
        }

        /// Write property map to stream
        void toStream(QDataStream&) const;

        /// Read property map from stream
        void fromStream(QDataStream&);

      private:
        typedef std::map<QString, QByteArray>property_map;

        /// Make MD5 checksum from byte array
        static QString makeChecksum(QByteArray const& _b);

        /// Make MD5 checksum from byte array
        static QString makeChecksum(QString const& _s);

        /// Make MD5 checksum from property map
        static QString makeChecksum(property_map const& _properties);

        property_map properties_;
    };
  }

  using serialization::PropertyMap;
}

OMNI_DECL_STREAM_OPERATORS(omni::serialization::PropertyMap)


#endif /* OMNI_SERIALIZATION_PROPERTYMAP_H_ */

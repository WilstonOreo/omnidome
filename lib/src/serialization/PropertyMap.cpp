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

#include <QCryptographicHash>

#include <QDebug>
#include <omni/serialization/PropertyMap.h>


namespace omni {
    namespace serialization {
        PropertyMap::PropertyMap()
        {
        }

        PropertyMap::PropertyMap(QDataStream& _stream) {
          fromStream(_stream);
        }

        void PropertyMap::toStream(QDataStream& _os) const {

            serialize(_os,makeChecksum(properties_));
            _os << uint32_t(properties_.size());

            for (auto& _property : properties_) {
                serialize(_os,makeChecksum(_property.second));
                serialize(_os,_property.first);
                serialize(_os,_property.second);
            }
        }

        void PropertyMap::fromStream(QDataStream& _is) {
            QString _hash;
            deserialize(_is,_hash);
            uint32_t _size = 0;
            _is >> _size;

            for (size_t i = 0; i < _size; ++i) {
                QString _propertyHash;
                QString _propertyId;
                QByteArray _propertyData;
                deserialize(_is,_propertyHash);
                deserialize(_is,_propertyId);
                deserialize(_is,_propertyData);
                properties_[_propertyId] = _propertyData;

                if (_propertyHash != makeChecksum(_propertyData)) {
                    throw exception::ChecksumError(_propertyHash, makeChecksum(
                                                       properties_), _propertyId);
                }
            }

            if (_hash != makeChecksum(properties_)) {
                throw exception::ChecksumError(_hash, makeChecksum(properties_));
            }
        }

        QString PropertyMap::makeChecksum(QByteArray const& _b) {
            return QString(QCryptographicHash::hash(_b,QCryptographicHash::Sha1).toHex());
        }

        QString PropertyMap::makeChecksum(QString const& _s) {
            return makeChecksum(_s.toLatin1());
        }

        QString PropertyMap::makeChecksum(property_map const& _properties) {
                QString _merged;
                _merged += QString("%1").arg(_properties.size());
                for (auto& _property : _properties) {
                    _merged += _property.first;
                    _merged += makeChecksum(_property.second);
                }
                return makeChecksum(_merged);
        }
    }
}

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

#include <omni/PluginInfo.h>

#include <QDebug>
#include <QFileInfo>
#include <QMetaClassInfo>

namespace omni {

    QString const& PluginInfo::name() const {
        return name_;
    }

    QString const& PluginInfo::type() const {
        return type_;
    }

    QString const& PluginInfo::author() const {
        return author_;
    }

    QString const& PluginInfo::url() const {
        return url_;
    }

    QString const& PluginInfo::description() const {
        return description_;
    }

    QString const& PluginInfo::className() const {
        return className_;
    }

    QString PluginInfo::versionString() const {
        return QString("%1.%2.%3")
            .arg(majorVersion())
            .arg(minorVersion())
            .arg(revision())
            .arg(build());
    }

    int PluginInfo::majorVersion() const {
        return majorVersion_;
    }

    int PluginInfo::minorVersion() const {
        return minorVersion_;
    }

    int PluginInfo::revision() const {
        return revision_;
    }

    int PluginInfo::build() const {
        return build_;
    }

    QString const& PluginInfo::file() const {
        return file_;
    }

    PluginInfo PluginInfo::make(QString const& _file, QObject const* _obj) {
        PluginInfo _info;
        _info.file_ = _file;
        _info.className_ = _obj->metaObject()->className();

        auto _getClassInfo = [&](QString const& _name) {
            auto const* _metaObject = _obj->metaObject();
            for (int i = 0; i < _metaObject->classInfoCount(); ++i) {
                auto _metaInfo = _metaObject->classInfo(i);
                if (QString(_metaInfo.name()) == _name) {
                    return QString(_metaInfo.value());
                }
            }
            return QString();
        };

        _info.name_ = _getClassInfo("name");
        if (_info.name_.isEmpty()) {
            QFileInfo fileInfo(_info.file_);
            _info.name_ = fileInfo.fileName();
        }

        _info.type_ = _getClassInfo("type");
        _info.author_ = _getClassInfo("author");
        _info.url_ = _getClassInfo("url");
        _info.description_ = _getClassInfo("description");
        QString _versionString = _getClassInfo("version");
        QStringList _tokens = _versionString.split( ".", QString::SkipEmptyParts);
        if (_tokens.size() >= 1) _info.majorVersion_ = _tokens[0].toInt();
        if (_tokens.size() >= 2) _info.minorVersion_ = _tokens[1].toInt();
        if (_tokens.size() >= 3) _info.revision_ = _tokens[2].toInt();
        if (_tokens.size() >= 4) _info.build_ = _tokens[3].toInt();
        return _info;
    }




}

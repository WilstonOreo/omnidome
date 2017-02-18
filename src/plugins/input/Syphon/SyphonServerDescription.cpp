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

#include "SyphonServerDescription.h"

namespace omni {
  namespace input {
    SyphonServerDescription::SyphonServerDescription(
      QString const& _appName,
      QString const& _serverName) :
        appName_(_appName),
        serverName_(_serverName) {}

    void SyphonServerDescription::put(QString const& _appName,
                                      QString const& _serverName) {
      appName_    = _appName;
      serverName_ = _serverName;
    }

    QString const& SyphonServerDescription::serverName() const {
      return serverName_;
    }

    QString const& SyphonServerDescription::applicationName() const {
      return appName_;
    }

    const char * SyphonServerDescription::serverNameAsConstChar() const {
      return serverName().toUtf8().constData();
    }

    const char* SyphonServerDescription::applicationNameAsConstChar() const {
      return applicationName().toUtf8().constData();
    }
  }
}

/// Serialize omni::input::ServerDescription to stream
QDataStream& operator<<(QDataStream& _os,
                        const omni::input::SyphonServerDescription& _p) {
  _os << _p.serverName() << _p.applicationName();
  return _os;
}

/// Deserialize omni::input::ServerDescription from stream
QDataStream& operator>>(QDataStream& _is,
                        omni::input::SyphonServerDescription& _p) {
  QString _serverName;
  QString _applicationName;
  _is >> _serverName >> _applicationName;
  _p.put(_applicationName,_serverName);
  return _is;
}

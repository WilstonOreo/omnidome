/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
      QString const& _serverName,
      QString const& _appName) : serverName_(_serverName), appName_(_appName) {}

    void SyphonServerDescription::put(QString const& _serverName,
                                      QString const& _appName) {
      serverName_ = _serverName;
      appName_    = _appName;
    }

    QString const& SyphonServerDescription::serverName() const {
      return serverName_;
    }

    QString const& SyphonServerDescription::applicationName() const {
      return appName_;
    }

    const char * SyphonServerDescription::serverNameAsConstChar() const {
      return serverName().toLatin1().data();
    }

    const char* SyphonServerDescription::applicationNameAsConstChar() const {
      return applicationName().toLatin1().data();
    }
  }
}

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

#ifndef OMNI_INPUT_SYPHONSERVERDESCRIPTION_H_
#define OMNI_INPUT_SYPHONSERVERDESCRIPTION_H_

#include <QString>
#include <QObject>
#include "SyphonServerDescription.h"

namespace omni {
  namespace input {
    /// A syphon server is identified by a server name and application name
    class SyphonServerDescription {
      public:
        /// Construct new server description with optional server and app name
        SyphonServerDescription(QString const& _appName = "null",
                                QString const& _serverName = "null");

        /// Test for equality
        inline friend bool operator==(const SyphonServerDescription& lhs,
                               const SyphonServerDescription& rhs) {
          return (lhs.serverName() == rhs.serverName()) &&
                 (lhs.applicationName() == rhs.applicationName());
        }

        /// Set server and application name
        void put(QString const& _appName, QString const& _serverName);

        /// Return server name
        QString const& serverName() const;

        /// Return application name
        QString const& applicationName() const;

        /**@brief Return server name as const char*
           @detail for simpler converting to NSString*
         **/
        const char* serverNameAsConstChar() const;

        /**@brief Return application name as const char*
           @detail for simpler converting to NSString*
         **/
        const char* applicationNameAsConstChar() const;

      private:
        QString appName_, serverName_;
    };
  }
}

/// Serialize omni::input::ServerDescription to stream
QDataStream& operator<<(QDataStream& _os,
                        const omni::input::SyphonServerDescription& _p);

/// Deserialize omni::input::ServerDescription from stream
QDataStream& operator>>(QDataStream& _is,
                        omni::input::SyphonServerDescription& _p);

#endif /* OMNI_INPUT_SYPHONSERVERDESCRIPTION_H_ */

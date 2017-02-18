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

#ifndef OMNI_INPUT_SYPHONSERVERDIRECTORY_H_
#define OMNI_INPUT_SYPHONSERVERDIRECTORY_H_

#include <QObject>
#include <CoreFoundation/CFDictionary.h>
#include <algorithm>
#include <vector>
#include "SyphonServerDescription.h"

namespace omni {
  namespace input {
    typedef std::vector<SyphonServerDescription> ServerList;

    class SyphonServerManager : public QObject {
      Q_OBJECT
      public:
        SyphonServerManager();
        ~SyphonServerManager();

        void                                  setup();
        bool                                  isSetup() const;
        int                                   size() const;

        bool                                  isValidIndex(int _idx) const;
        bool                                  serverExists(
          SyphonServerDescription const& _server) const;

        SyphonServerDescription getDescription(int _idx) const;

        std::vector<SyphonServerDescription> getServerList() const;
        void                                  printList() const;

        // needs to be public because of the nature of CFNotifications.  please
        // do not call this.
        void                                  handleNotification(CFStringRef name,
                                                                 CFDictionaryRef userInfo);

      signals:
        void serverAnnounced(ServerList const&);
        void serverUpdated(ServerList const&);
        void serverRetired(ServerList const&);

      private slots:
        void serverAnnouncedRefresh();
        void serverUpdatedRefresh();
        void serverRetiredRefresh();

      private:
        void refresh(bool isAnnounce);

        void addObservers();
        void removeObservers();

        bool CFStringRefToString(CFStringRef src,
                                 QString& dest);

        bool isSetup_ = false;
        ServerList serverList_;
    };
  }
}


#endif /* OMNI_INPUT_SYPHONSERVERDIRECTORY_H_ */

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

#ifndef OMNI_MEDIA_INTERFACE_H_
#define OMNI_MEDIA_INTERFACE_H_

#include <QString>
#include <QDataStream>
#include <omni/exception.h>

namespace omni {
  namespace media {
    /// Base interface for loading external media files
    class OMNI_EXPORT Location {
      public:
        Location();
        Location(QString const&);

        template<typename MEDIA>
        void reload(MEDIA& _media) {
          load(path_,_media);
        }

        /// Load media from file
        template<typename MEDIA>
        void load(QString const& _path, MEDIA& _media) {
          setPath(_path);
          _media.load(_path);
        }

        bool           exists() const;
        static bool    exists(QString const&);

        uint64_t       fileSize() const;
        QString const& path() const;
        void           setPath(QString const&);

        /// Relocate media by searching directories, with optional directory hint
        void           relocate(QString const& _directoryHint = QString());

      private:
        qint64   fileSize_ = 0;
        QString  path_;
    };

    namespace exception {
      /// Exception which is thrown when a media does not exist in location
      class NotExisting : public Error {
        public:
          OMNI_EXCEPTION(NotExisting)

          NotExisting(Location& _location) :
            location_(_location) {}

          inline QString message() const throw() {
            QString _msg("Location %1 does not exist!");
            _msg = _msg.arg(location_.path());
            return _msg;
          }

        private:
          Location& location_;
      };
    }

  }
}

QDataStream& OMNI_EXPORT operator>>(QDataStream&,
                        omni::media::Location&);
QDataStream& OMNI_EXPORT operator<<(QDataStream&,
                        omni::media::Location const&);


#endif /* OMNI_MEDIA_INTERFACE_H_ */

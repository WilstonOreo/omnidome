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

#include <omni/media/Location.h>

#include <QDebug>
#include <QFileInfo>

namespace omni {
  namespace media {
    Location::Location() {}

    Location::Location(QString const & _path) {
      setPath(_path);
    }

    bool Location::exists() const {
      return exists(path_);
    }

    bool Location::exists(QString const& _path) {
      QFileInfo _fileInfo(_path);
      return _fileInfo.exists();
    }

    QString const& Location::path() const {
      return path_;
    }

    void Location::relocate(QString const& _directoryHint) {
      /*
            QDirIterator dirIt("/folder_path",QDirIterator::Subdirectories);
         while (dirIt.hasNext()) {
         dirIt.next();
         if (QFileInfo(dirIt.filePath()).isFile())
       */
    }

    void Location::setPath(QString const& _path) {
      path_ = _path;
      QFileInfo _fileInfo(_path);
      fileSize_ = _fileInfo.size();
    /*  if (!exists() && !path_.isEmpty()) {
        throw exception::NotExisting(*this);
      }*/
    }
  }
}

QDataStream& operator>>(QDataStream& _is,
                        omni::media::Location& _loc) {
  QString _string;
  _is >> _string;
  _loc.setPath(_string);
  return _is;
}

QDataStream& operator<<(QDataStream& _os,
                        omni::media::Location const& _loc) {
  _os << _loc.path();
  return _os;
}

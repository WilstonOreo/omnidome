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

#include <omni/util.h>

#include <QFile>
#include <QFileInfo>

namespace omni {
  namespace util {

    /// Return content of file from a file name
    QString fileToStr(const QString& _filename)
    {
      QFile _f(_filename);

      _f.open(QIODevice::ReadOnly | QIODevice::Text);
      return _f.readAll();
    }

    QString removeFileExt(QString const& _filename)
    {
      int _lastIndex = _filename.lastIndexOf(".");

      if (_lastIndex == -1) return _filename;

      QString _rawName;

      for (int i = 0; i < _lastIndex; ++i) _rawName.push_back(_filename[i]);
      return _rawName;
    }

    bool fileExists(QString const& _filename) {
      QFileInfo checkFile(_filename);

      // check if file exists and if yes: Is it really a file and no directory?
      if (checkFile.exists() && checkFile.isFile()) return true;

      return false;
    }
  }
}

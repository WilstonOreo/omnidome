/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#ifndef OMNI_UI_RECENTSESSIONS_H_
#define OMNI_UI_RECENTSESSIONS_H_

#include <deque>
#include <QMenu>
#include <QString>
#include <omni/util.h>

class QFileSystemWatcher;

namespace omni {
  namespace ui {
    class RecentFileAction : public QAction {
        Q_OBJECT
      public:
        RecentFileAction(QString const& _file, QObject* = nullptr);
        ~RecentFileAction();

        bool fileExists() const;

      signals:
        void fileToBeLoaded(QString const&);

      public slots:
        void loadSession();
        void setState();

      private:
        QString file_;
        QUniquePtr<QFileSystemWatcher> watcher_;
    };

    /// Data structure and menu to store default sessions
    class RecentSessions : public QObject {
        Q_OBJECT
      public:
        RecentSessions(QObject * = nullptr);
        ~RecentSessions();

        QMenu* menu();
        void   readFromSettings();
        void   writeToSettings() const;
        int    numberOfFiles() const;

      public slots:
        void   clear();
        void   addFile(QString const&);
        void   setNumberOfFiles(int);

      signals:
        void fileToBeLoaded(QString const&);

      private:
        void   addFile(QString const&, bool _regenerateMenu);

        /// Return index of file if in list, return -1 if not
        int  fileInList(QString const&) const;

        void generateMenu();

        std::deque<QString> recentFiles_;
        QMenu *menu_          = nullptr;
        int    numberOfFiles_ = 16;
    };
  }
}

#endif /* OMNI_RECENTSESSIONS_H_ */

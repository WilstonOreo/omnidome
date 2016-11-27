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

#ifndef OMNI_UI_RECENTSESSIONS_H_
#define OMNI_UI_RECENTSESSIONS_H_

#include <deque>
#include <QMenu>
#include <QString>
#include <omni/util.h>

namespace omni {
  namespace ui {
    /// Action for recent file
    class RecentFileAction : public QAction {
        Q_OBJECT
      public:
        RecentFileAction(QString const& _file, QObject* = nullptr);
        ~RecentFileAction();

        /// Check if file exists
        bool fileExists() const;

      signals:
        void fileToBeLoaded(QString const&);

      public slots:
        /// Load session triggers fileToBeLoaded signal
        void loadSession();

        /// Action is disabled when file does not exist
        void setState();

      private:
        QString file_;
    };

    /// Data structure and menu to store default sessions
    class RecentSessions : public QObject {
        Q_OBJECT
      public:
        RecentSessions(QObject * = nullptr);
        ~RecentSessions();

        /// Return pointer to menu
        QMenu* menu();

        /// Read recent files from settings
        void   readFromSettings();

        /// Write recent files to settings
        void   writeToSettings() const;

        /// Return maximum number of files
        int    numberOfFiles() const;

      public slots:
        /// Clear list of recent files
        void   clear();

        /// Add a new file to list
        void   addFile(QString const&);

        /// Set maximum number of files
        void   setNumberOfFiles(int);

        /// Disable action for which file does not exists
        void   setActionStates();

      signals:
        void fileToBeLoaded(QString const&);

      private:
        /// Add file and optionally regenerate menu
        void   addFile(QString const&, bool _regenerateMenu);

        /// Return index of file if in list, return -1 if not
        int  fileInList(QString const&) const;

        /// Generate menu from recent files, create new menu if necessary
        void generateMenu();

        std::deque<QString> recentFiles_;
        std::vector<RecentFileAction*> actions_;
        QUniquePtr<QMenu> menu_;
        int    numberOfFiles_ = 16;
    };
  }
}

#endif /* OMNI_RECENTSESSIONS_H_ */

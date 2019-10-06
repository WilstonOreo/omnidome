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

#include <QObject>
#include <QApplication>
#include <QSettings>

namespace omni {
  namespace ui {
    /// Omnidome Application
    class Application : public QApplication {
        Q_OBJECT
      public:
        /// Construct with command arguments and load plugins
        Application(int& ac,
                    char **av);
        ~Application();

        /// Set custom stylesheet file
        void              setStyleSheetFile(QString _file);

        /// Return string to style sheet file
        QString           styleSheetFile() const;

        /// Application settings to be saved
        static QSettings& settings();

      private:
        bool              eventFilter(QObject *object,
                                      QEvent *event);

        void              loadPlugins();

        QString styleSheetFile_;
        static QSettings settings_;
    };
  }
}

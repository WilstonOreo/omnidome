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

#include "Application.h"
#include <QKeyEvent>
#include <QDebug>
#include <QFontDatabase>
#include <QSurfaceFormat>
#include <omni/PluginLoader.h>

namespace omni {
    namespace ui {
        QSettings Application::settings_("omnidome.settings");

        Application::Application(int& ac, char **av) :
            QApplication(ac, av)
        {
            QCoreApplication::setApplicationName("Omnidome");
            QCoreApplication::setApplicationVersion(OMNI_VERSION_STRING);
            QCoreApplication::setOrganizationName("Michael Winkelmann");
            QCoreApplication::setOrganizationDomain("Michael Winkelmann.org / omnido.me");
            loadPlugins();

            std::vector<QString> _fonts = {
                ":/fonts/Dosis-Bold.ttf",
                ":/fonts/Dosis-Light.ttf",
                ":/fonts/Dosis-Regular.ttf"
            };

            for (auto& _font : _fonts) {
                int id = QFontDatabase::addApplicationFont(_font);
#ifdef DEBUG
//		QString family = QFontDatabase::applicationFontFamilies(id).at(0);
//                qDebug() << family;
#endif
            }

            setStyleSheetFile(":/stylesheet.qss");

            installEventFilter(this);
        }

        Application::~Application() {}

        void Application::setStyleSheetFile(QString _styleSheetFile) {
            // Load application style sheet
            QFile file(_styleSheetFile);

            file.open(QFile::ReadOnly);
            QString styleSheet = QLatin1String(file.readAll());
            setStyleSheet(styleSheet);
            styleSheetFile_ = _styleSheetFile;
        }

        QString Application::styleSheetFile() const {
            return styleSheetFile_;
        }

        QSettings& Application::settings() {
            return settings_;
        }

        void Application::loadPlugins() {

            std::vector<QDir> _pluginDirs;

            // Load plugins
            PluginLoader _pluginLoader(_pluginDirs);
        }

        bool Application::eventFilter(QObject *object, QEvent *event)
        {
#ifdef DEBUG // Reload style sheet in debug mode when pressing F5
            if (event->type() == QEvent::KeyPress)
            {
                auto *keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_F5) {
                    setStyleSheetFile(styleSheetFile_);
                }
            }
#endif // ifdef DEBUG
            return QObject::eventFilter(object, event);
        }
    }
}

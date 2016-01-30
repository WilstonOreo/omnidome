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


#include "MainWindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <omni/PluginLoader.h>

#include <QCommandLineParser>

using namespace std;
using namespace omni;

int main(int ac, char *av[])
{
    // This line is absolutely mandatory for being able to have multiple
    // QOpenGLWidgets in different windows!!!
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QCoreApplication::setApplicationName("Omnidome");
    QCoreApplication::setApplicationVersion(OMNIDOME_VERSION_STRING);

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();

    /// Start gui
    QApplication _a(ac, av);


    QFile file(":/qss_icons/style.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    _a.setStyleSheet(styleSheet);


    parser.addOptions({
        // An option with a value
        {{"p", "plugin-directory"},
            QCoreApplication::translate("main", "Plugin directory <directory>."),
            QCoreApplication::translate("main", "plugin-directory")},
    });

    parser.process(_a);
    const QStringList args = parser.positionalArguments();

    // Load plugins
    {
        std::vector<QDir> _pluginDirs;
        if (parser.value("plugin-directory").isEmpty()) {
            _pluginDirs.push_back(parser.value("plugin-directory"));
        }
        PluginLoader _pluginLoader(_pluginDirs);
    }

    ui::MainWindow _w;
    _w.setWindowState(Qt::WindowMaximized);
    _w.move(QApplication::primaryScreen()->geometry().topLeft());

    if (ac == 2)
    {
        _w.openProjection(av[1]);
    }

    return _a.exec();
}

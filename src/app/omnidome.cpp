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

    QString _styleSheetFile(":/qss_icons/style.qss");
    std::vector<QDir> _pluginDirs;

/// Command line parser is only available in debug mode
#ifdef DEBUG
    QCommandLineParser parser;
    parser.setApplicationDescription("Omnidome");

    /// Start gui
    QApplication _a(ac, av);

    parser.addOptions({
        // An option with a value
        {{"p", "plugin-directory"},
            QCoreApplication::translate("main", "Plugin directory <directory>."),
            QCoreApplication::translate("main", "plugin-directory")},
        {{"s", "stylesheet"},
            QCoreApplication::translate("main", "Style sheet file <filename>."),
            QCoreApplication::translate("main", "stylesheet")},
    });
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(_a);
    const QStringList args = parser.positionalArguments();

    if (!parser.value("plugin-directory").isEmpty()) {
        _pluginDirs.push_back(parser.value("plugin-directory"));
    }
#endif

    // Load application style sheet
    QFile file(_styleSheetFile);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    _a.setStyleSheet(styleSheet);

    // Load plugins
    PluginLoader _pluginLoader(_pluginDirs);

    ui::MainWindow _w;
    _w.setWindowState(Qt::WindowMaximized);
    _w.move(QApplication::primaryScreen()->geometry().topLeft());

// Load mapping session from given commandline argument when in release mode
#ifndef DEBUG
    if (ac == 2)
    {
        _w.openProjection(av[1]);
    }
#endif

    return _a.exec();
}

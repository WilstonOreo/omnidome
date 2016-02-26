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

#include "Application.h"
#include <QDir>
#include <QFile>
#include <omni/PluginLoader.h>

#include <QCommandLineParser>

using namespace omni;

class CommandLineParser {
public:
    void parse(QApplication const& _app) {

        QString _argument;
        for (auto& _arg : _app.arguments()) {
            if (_arg[0] == '+') {
                _argument = "";
                for (int i = 1; i < _arg.size(); ++i) {
                    _argument += _arg[i];
                }
            } else {
                keyValues_[_argument] += _arg + " ";
            }
        }

        for (auto& _keyValue : keyValues_) {
            _keyValue.second = _keyValue.second.trimmed();

        }
    }

    QString value(QString const& _key) const {
        return keyValues_.at(_key);
    }

private:

    std::map<QString,QString> keyValues_;

};

int main(int ac, char *av[])
{
            QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

            QCoreApplication::setApplicationName("Omnidome");
            QCoreApplication::setApplicationVersion(OMNIDOME_VERSION_STRING);
    std::vector<QDir> _pluginDirs;
    omni::ui::Application _a(ac, av);

/// Command line parser is only available in debug mode
#ifdef DEBUG

    CommandLineParser parser;
    parser.parse(_a);

/*
    parser.setApplicationDescription("Omnidome");


    QCommandLineOption _pluginDirOption({"p", "plugin-directory"},
            QCoreApplication::translate("main", "Plugin directory <directory>."),
            QCoreApplication::translate("main", "plugin-directory"));

    QCommandLineOption _styleSheetOption({"s", "stylesheet"},
            QCoreApplication::translate("main", "Style sheet file <filename>."),
            QCoreApplication::translate("main", "stylesheet"));

    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(_pluginDirOption);
    parser.addOption(_styleSheetOption);

    parser.process(_a);

    //const QStringList args = parser.positionalArguments();

    if (!parser.value(_pluginDirOption).isEmpty()) {
        _pluginDirs.push_back(parser.value("plugin-directory"));
    }
*/
    if (!parser.value("stylesheet").isEmpty()) {
        qDebug() << parser.value("stylesheet");
        _a.setStyleSheetFile(parser.value("stylesheet"));
    }
#endif

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

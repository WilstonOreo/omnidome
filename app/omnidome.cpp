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


#include "MainWindow.h"

#include "Application.h"
#include <QDir>
#include <QFile>

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
      if (!keyValues_.count(_key)) {
        return QString();
      }

      return keyValues_.at(_key);
    }

  private:
    std::map<QString, QString> keyValues_;
};

int main(int ac, char *av[])
{
  // This line is absolutely mandatory for being able to have multiple
  // QOpenGLWidgets in different windows!!!
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QSurfaceFormat _format;

//  _format.setVersion(3,3);
  _format.setProfile(QSurfaceFormat::CompatibilityProfile);
#ifdef QT_DEBUG
  _format.setOption(QSurfaceFormat::DebugContext);
#endif
  QSurfaceFormat::setDefaultFormat(_format);


  omni::ui::Application _a(ac, av);

  /// Command line parser is only available in debug mode
#ifdef QT_DEBUG
  CommandLineParser parser;
  parser.parse(_a);

  if (!parser.value("stylesheet").isEmpty()) {
    qDebug() << parser.value("stylesheet");
    _a.setStyleSheetFile(parser.value("stylesheet"));
  }
#endif // ifdef DEBUG

  ui::MainWindow _w;
  _w.move(QApplication::primaryScreen()->geometry().topLeft());

#if !defined (Q_OS_WIN)
  Q_INIT_RESOURCE(libomni);
#endif
  // Load mapping session from given commandline argument when in release mode
#ifndef QT_DEBUG

  if (ac == 2)
  {
    _w.openProjection(av[1]);
  }
#endif // ifndef DEBUG

  return _a.exec();
}

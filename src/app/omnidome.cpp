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


#include <omni/ui/MainWindow.h>

#include <QApplication>
#include <QFile>

using namespace std;
using namespace omni;

int main(int ac, char* av[])
{
  // This line is absolutely mandatory for being able to have multiple
  // QOpenGLWidgets in different windows!!!
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  /// Start gui
  QApplication _a(ac, av);
   
  QFile file(":/qss_icons/style.qss");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  _a.setStyleSheet(styleSheet);

  ui::MainWindow _w;
  _w.setWindowState(Qt::WindowMaximized);
  _w.move(QApplication::primaryScreen()->geometry().topLeft());
  if (ac > 1)
  {
    _w.openProjection(av[1]);
  } 
  
  return _a.exec();
}

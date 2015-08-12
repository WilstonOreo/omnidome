
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
  _w.showFullScreen();
  if (ac > 1)
  {
    _w.openProjection(av[1]);
  } 
  
  return _a.exec();
}

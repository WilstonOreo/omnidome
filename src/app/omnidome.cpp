
#include <omni/ui/MainWindow.h>

#include <QApplication>
#include <QFile>
#include <QtOpenGL/QGLFormat>

using namespace std;
using namespace omni;

int main(int ac, char* av[])
{

  QGLFormat f;
  f.setDoubleBuffer(false);
  f.setVersion(3,3);
  QGLFormat::setDefaultFormat(f);

  /// Start gui
  QApplication _a(ac, av);
   
  QFile file(":/qss_icons/style.qss");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  _a.setStyleSheet(styleSheet);

  ui::MainWindow _w;
  _w.setWindowState(Qt::WindowMaximized);

  if (ac > 1)
  {
    _w.openProjection(av[1]);
  } 
  
  return _a.exec();
}

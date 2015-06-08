
#include <omni/ui/ScreenSetup.h>

#include <QApplication>
#include <QFile>

using namespace std;
using namespace omni;

int main(int ac, char* av[])
{
  /// Start gui
  QApplication _a(ac, av);
  ui::ScreenSetup _s;

  _s.show();

  return _a.exec();
}

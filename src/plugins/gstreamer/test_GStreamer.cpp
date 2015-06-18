
#include <QApplication>
#include "GLDisplay.h"

/// Test for projector widget
int main(int ac, char* av[])
{
  /// Start gui
  QApplication _a(ac, av);
 
  omni::ui::GLDisplay _disp;

  _disp.setPort(5000);

  _disp.show();

  return _a.exec();
}


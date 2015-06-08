
#include <omni/ui/GLView3D.h>

#include <omni/Session.h>

#include <QApplication>

using namespace std;
using namespace omni;

int main(int ac, char* av[])
{
  /// Start gui
  QApplication _a(ac, av);

  Session _session;

  for (int i = 0; i < 4; ++i)
  {
    auto* _tuning = _session.tunings().add();

    Q_ASSERT(_tuning);
  }
  
  ui::GLView3D _view;

  _view.show();
  _view.setSession(&_session);

  return _a.exec();
}


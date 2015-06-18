
#include <omni/ui/GLView3D.h>

#include <omni/Session.h>
#include <omni/canvas/HalfDome.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>
#include <omni/mapping/Equirectangular.h>

#include <QApplication>

int main(int ac, char* av[])
{
  using namespace omni;
  /// Start gui
  QApplication _a(ac, av);

  canvas::Factory::reg<canvas::HalfDome>();
  input::Factory::reg<input::Image>();

  Session _session;

  _session.inputs().add<input::Image>("image",":/spherical.jpg");
  _session.inputs().add<input::TestImage>("test_image");
  
  _session.inputs().setCurrent("test_image");

  Q_ASSERT(_session.setCanvas("HalfDome"));

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


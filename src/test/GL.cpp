
#include <omni/ui/GLView3D.h>

#include <omni/Session.h>
#include <omni/canvas/HalfDome.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>
#include <omni/mapping/Equirectangular.h>
#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>

#include <QDataStream>
#include <QFile>
#include <QApplication>

int main(int ac, char* av[])
{
  using namespace omni;
  /// Start gui
  QApplication _a(ac, av);

  Session _session;

  _session.inputs().add<input::Image>("image",":/spherical.jpg");
  _session.inputs().add<input::TestImage>("test_image");
  
  _session.inputs().setCurrent("test_image");

  for (int i = 0; i < 18; ++i)
  {
    auto* _tuning = _session.tunings().add();
  
    auto* _projSetup = static_cast<proj::PeripheralSetup*>(_tuning->setupProjector("PeripheralSetup"));

    _projSetup->setYaw(i*20);
    _projSetup->setPitch(i*5);
    _projSetup->setDistanceCenter(9.0);
    _projSetup->setup(const_cast<proj::Projector&>(_tuning->projector()));
  }

  Q_ASSERT(_session.setCanvas("HalfDome"));

  
  ui::GLView3D _view;

  _view.show();
  _view.setSession(&_session);


  return _a.exec();
}


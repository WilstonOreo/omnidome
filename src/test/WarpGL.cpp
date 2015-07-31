#include <omni/ui/TuningGLView.h>

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
  _session.inputs().add<input::Image>(":/spherical.jpg");
  _session.inputs().setCurrentIndex(0);

  auto* _tuning = _session.tunings().add();
  auto* _projSetup = static_cast<proj::PeripheralSetup*>(_tuning->setupProjector("PeripheralSetup"));

  _tuning->setColor("#FF0000");

  _projSetup->setYaw(0);
  _projSetup->setPitch(30);
  _projSetup->setDistanceCenter(9.0);
  _projSetup->setup(const_cast<proj::Projector&>(_tuning->projector()));

  auto* _canvas = _session.setCanvas("HalfDome");
  _canvas->update();

  auto* _input = _session.inputs().current();
  qDebug() << _input->getTypeId();

  _session.setMode(Session::Mode::BLEND);

  ui::TuningGLView _view;
  _view.show();
  _view.setSession(&_session);
  _view.setKeepAspectRatio(true);
  _view.setTuningIndex(0);

  return _a.exec();
}


#include <omni/ui/TuningGLView.h>

#include <omni/Session.h>
#include <omni/canvas/HalfDome.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>
#include <omni/mapping/Equirectangular.h>
#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>
#include <omni/Renderer.h>

#include <QDataStream>
#include <QFile>
#include <QApplication>
#include <QRectF>

int main(int ac, char* av[])
{
  using namespace omni;
  /// Start gui
  QApplication _a(ac, av);

  Session _session;
  _session.inputs().add<input::Image>("/home/wilston/Dropbox/peacock_test.jpg");
  _session.inputs().setCurrentIndex(0);

  auto* _tuning = _session.tunings().add();
  auto* _projSetup = static_cast<proj::PeripheralSetup*>(_tuning->projector().setup("PeripheralSetup"));

  _tuning->setColor("#FF0000");
  _tuning->blendMask().setRect(QRectF(0.2,0.2,0.6,0.6));

  _projSetup->setYaw(0);
  _projSetup->setPitch(-90.0);
  _projSetup->setDistanceCenter(0.0);
  _projSetup->setTowerHeight(8.0);
  _tuning->projector().setup();

  auto* _canvas = _session.setCanvas("HalfDome");
  _canvas->update();

  auto* _input = _session.inputs().current();
  qDebug() << _input->getTypeId();

  _input->update();

  _session.setMode(Session::Mode::WARP);
  _session.setBlendMode(Session::BlendMode::WHITE);

  ui::TuningGLView _view;
  _view.show();
  _view.setSession(&_session);
  _view.setKeepAspectRatio(true);
  _view.setTuningIndex(0);

  RenderOptions _options;
  _options.setMappingOutputMode(Mapping::UVW);
  Renderer _renderer(_session,_options);

  QImage _image;
  _renderer.render(_tuning,_image);
  _image.save("renderer.png");

  return _a.exec();
}



#include <QGuiApplication>
#include <QCommandLineParser>
#include "projectionwindow.h"

#include <omni/Session.h>
#include <omni/proj/Tuning.h>

using namespace omni;

int main(int ac, char *av[]) {
  QGuiApplication app(ac, av);

  QCommandLineParser parser;
  parser.setApplicationDescription("Omnidome Server");
  parser.addHelpOption();
  parser.addVersionOption();

  parser.addOptions(
      {{{"config", "c"}, "Configuration file", "config"},
  });

  parser.process(app);

  Session session;

  session.setMode(Session::Mode::LIVE);

  ProjectionWindow window(&session);

  window.show();

  return app.exec();
}

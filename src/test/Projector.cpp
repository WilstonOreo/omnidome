#include <omni/Session.h>
#include <omni/proj/Screen.h>
#include <omni/proj/Projector.h>
#include <omni/ui/proj/TuningList.h>

#include <QApplication>

/// Test for projector widget
int main(int ac, char* av[])
{
  /// Start gui
  QApplication _a(ac, av);
  
  omni::Session _session;

  omni::ui::proj::TuningList _tuningListWidget;

  _tuningListWidget.setSession(&_session);

  _tuningListWidget.addTuning();
  _tuningListWidget.addTuning();
  _tuningListWidget.addTuning();

  _tuningListWidget.show();
  
  return _a.exec();
}


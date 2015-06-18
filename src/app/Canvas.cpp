#include <omni/ui/Canvas.h>

#include "ui_omni_ui_Canvas.h"

namespace omni
{
  namespace ui
  {
    Canvas::Canvas(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Canvas)
    {
      ui_->setupUi(this);
    }

    Canvas::~Canvas()
    {
    }

    Session const* Canvas::session() const
    {
      return session_;
    }

    void Canvas::setSession(Session* _session)
    {
      session_=_session;
    }
  }
}

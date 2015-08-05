#include <omni/ui/Warp.h>

#include "ui_omni_ui_Warp.h"

namespace omni
{
  namespace ui
  {
    Warp::Warp(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Warp)
    {
      ui_->setupUi(this);
    }

    Warp::~Warp()
    {
    }

    Session const* Warp::session() const
    {
      return session_;
    }

    void Warp::setSession(Session* _session)
    {
      session_=_session;
    }
  }
}

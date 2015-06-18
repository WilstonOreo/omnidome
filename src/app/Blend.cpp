#include <omni/ui/Blend.h>

#include "ui_omni_ui_Blend.h"

namespace omni
{
  namespace ui
  {
    Blend::Blend(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Blend)
    {
      ui_->setupUi(this);
    }

    Blend::~Blend()
    {
    }

    Session const* Blend::session() const
    {
      return session_;
    }

    void Blend::setSession(Session* _session)
    {
      session_=_session;
    }
  }
}

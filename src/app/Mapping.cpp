#include <omni/ui/Mapping.h>

#include "ui_omni_ui_Mapping.h"

namespace omni
{
  namespace ui
  {
    Mapping::Mapping(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Mapping)
    {
      ui_->setupUi(this);
    }

    Mapping::~Mapping()
    {
    }

    Session const* Mapping::session() const
    {
      return session_;
    }

    void Mapping::setSession(Session* _session)
    {
      session_=_session;
    }
  }
}

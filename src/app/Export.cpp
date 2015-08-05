#include <omni/ui/Export.h>

#include <omni/Session.h>

#include "ui_omni_ui_Export.h"

namespace omni
{
  namespace ui
  {
    Export::Export(QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
    }
    
    Export::Export(Session* _session, QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
      setSession(_session);
    }

    Export::~Export()
    {
    }
      
    Session const* Export::session() const
    {
      return session_;
    }
  
    void Export::setSession(Session* _session)
    {
      session_=_session;
    }
  }
}

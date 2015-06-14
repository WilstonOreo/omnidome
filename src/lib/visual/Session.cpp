#include <omni/visual/Session.h>

namespace omni
{
  namespace visual
  {
    Session::Session(omni::Session const& _session) : 
      session_(_session)
    {
    }
      
    omni::Session const& Session::session()
    {
      return session_;
    }
      
    void Session::drawCanvas() const
    {
      auto _canvas = session_.canvas();

      if (!_canvas) return; 

      _canvas->draw();
    }
  }
}

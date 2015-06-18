#include <omni/visual/Session.h>

namespace omni
{
  namespace visual
  {
    Session::Session(omni::Session& _session) : 
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


      auto* _input = session_.inputs().current();
      if (_input) _input->bind();

      _canvas->draw();

      if (_input) _input->release();
    }

    void Session::update()
    {
      auto* _canvas = session_.canvas();
      if (!_canvas) return;

      _canvas->update();

      auto* _input = session_.inputs().current();
      if (_input) _input->update();
    }

    void Session::free()
    {
      auto* _input = session_.inputs().current();
      if (_input) _input->free();
    }
  }
}

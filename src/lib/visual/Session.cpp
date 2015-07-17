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
      auto* _mapping = session_.mapping();

      qDebug() << _input << " " << _mapping;

      if (_input) _input->bind(_mapping);

      _canvas->draw();

      if (_input) _input->release(_mapping);
    }
      
    void Session::drawProjectors() const
    {
      for (auto& _proj : projectors_)
        _proj.draw();
    }
      
    void Session::drawProjectorHalos() const
    {
      for (auto& _proj : projectors_)
        _proj.drawHalo();
    }

    void Session::update()
    {
      auto* _canvas = session_.canvas();
      if (!_canvas) return;

      _canvas->update();

      auto* _input = session_.inputs().current();
      if (_input) _input->update();

      // Update projector visualizers
      projectors_.clear();
      for (int i = 0; i < session_.tunings().size(); ++i)
      {
        projectors_.emplace_back(session_.tunings()[i]->projector());
        projectors_.back().setColor(session_.tunings()[i]->color());
        projectors_.back().update();
      }
    }

    void Session::free()
    {
      auto* _input = session_.inputs().current();
      if (_input) _input->free();
    }
  }
}

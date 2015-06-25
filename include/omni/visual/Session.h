#ifndef OMNI_VISUAL_SESSION_H_
#define OMNI_VISUAL_SESSION_H_

#include <list>
#include <omni/Session.h>
#include <omni/visual/Projector.h>

namespace omni
{
  namespace visual
  {
    /// Base class for drawing a session
    class Session 
    {
    public:
      Session(omni::Session&);

      omni::Session const& session();

      void drawCanvas() const;

      void update();

      void free();

      void drawProjectors() const;
      void drawProjectorHalos() const;

    private:

      omni::Session& session_;

      std::list<visual::Projector> projectors_;
    };

  }
}

#endif /* OMNI_VISUAL_SESSION_H_ */

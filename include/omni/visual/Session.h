#ifndef OMNI_VISUAL_SESSION_H_
#define OMNI_VISUAL_SESSION_H_

#include <omni/Session.h>

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

    private:

      omni::Session& session_;
    };

  }
}

#endif /* OMNI_VISUAL_SESSION_H_ */

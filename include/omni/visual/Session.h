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
      Session(omni::Session const&);

      omni::Session const& session();

      void drawCanvas() const;

      

    private:

      omni::Session const& session_;
    };

  }
}

#endif /* OMNI_VISUAL_SESSION_H_ */

#ifndef OMNI_VISUAL_SESSION_H_
#define OMNI_VISUAL_SESSION_H_

#include <list>
#include <omni/Session.h>
#include <omni/visual/Projector.h>
#include <omni/visual/WarpGrid.h>

namespace omni
{
  namespace visual
  {
    /// Base class for drawing a session
    class Session 
    {
    public:
      Session(omni::Session&);

      omni::Session const& session() const;
      omni::Session& session();

      void drawCanvas() const;
      void drawCanvasWithFrustumIntersections() const;

      void update();
      void free();

      void drawProjectors() const;
      void drawProjectorHalos() const;

      bool needsUpdate() const;

    private:
      omni::Session& session_;
      static std::unique_ptr<QOpenGLShaderProgram> frustumShader_;

      std::list<visual::Projector> projectors_;
    };

  }
}

#endif /* OMNI_VISUAL_SESSION_H_ */

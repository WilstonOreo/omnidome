#include <omni/canvas/Envelope.h>

namespace omni
{
  namespace canvas
  {
    Envelope::Envelope()
    {
    }

    Envelope::~Envelope()
    {
    }
      
    void Envelope::draw() const
    {
      // Cullface according to projection mode
      switch (this->projectionMode())
      {
      case PROJECT_INSIDE: 
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
      case PROJECT_OUTSIDE:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
      case PROJECT_BOTH_SIDES:
        glDisable(GL_CULL_FACE);
      }
    }

    Envelope::ProjectionMode Envelope::projectionMode() const
    {
      return projectionMode_;
    }

    void Envelope::setProjectionMode(ProjectionMode _projectionMode)
    {
      projectionMode_ = _projectionMode;
      update();
    }

    Box Envelope::bounds() const
    {
      return bounds_;
    }
  }
}

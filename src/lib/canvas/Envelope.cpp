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

    bool Envelope::hasUVWMap() const
    {
      return true;
    }

    ProjectionMode Envelope::projectionMode() const
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

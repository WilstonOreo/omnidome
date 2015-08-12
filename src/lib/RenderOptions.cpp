#include <omni/RenderOptions.h>

namespace omni
{
  mapping::OutputMode RenderOptions::mappingOutputMode() const
  {
    return mappingOutputMode_;
  }

  void RenderOptions::setMappingOutputMode(mapping::OutputMode _outputMode)
  {
    mappingOutputMode_ = _outputMode;
  }
    
  SeparationMode RenderOptions::separationMode() const
  {
    return separationMode_;
  }
    
  void RenderOptions::setSeparationMode(SeparationMode _separationMode)
  {
    separationMode_ = _separationMode;
  }

  bool RenderOptions::excludeUnassignedProjectors() const
  {
    return excludeUnassignedProjectors_;
  }

  void RenderOptions::setExcludeUnassignedProjectors(bool _excludeUnassignedProjectors)
  {
    excludeUnassignedProjectors_ = _excludeUnassignedProjectors;
  }
}

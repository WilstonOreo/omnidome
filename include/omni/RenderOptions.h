#ifndef OMNI_RENDEROPTIONS_H_
#define OMNI_RENDEROPTIONS_H_

#include <omni/mapping/Interface.h>

namespace omni
{
  /// Mode how calibrations are stitched together
  enum class SeparationMode
  {
    NONE,
    SCREENS,
    PROJECTORS
  };

  /// Class for defining render options for calibration data
  class RenderOptions 
  {
  public:
    mapping::OutputMode mappingOutputMode() const;
    void setMappingOutputMode(mapping::OutputMode);

    SeparationMode separationMode() const;
    void setSeparationMode(SeparationMode);

    bool excludeUnassignedProjectors() const;
    void setExcludeUnassignedProjectors(bool); 

  private:
    mapping::OutputMode mappingOutputMode_ = mapping::OutputMode::TEXCOORDS;
    SeparationMode separationMode_ = SeparationMode::SCREENS;
    bool excludeUnassignedProjectors_ = false;
  };
}


#endif /* OMNI_RENDEROPTIONS_H_ */

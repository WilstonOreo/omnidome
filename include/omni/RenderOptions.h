#ifndef OMNI_RENDEROPTIONS_H_
#define OMNI_RENDEROPTIONS_H_

namespace omni
{
  enum class RenderMode
  {
    TEXCOORD,
    UVW,
    INPUT
  };

  enum class StitchMode
  {
  };
  
  class RenderOptions 
  {
  public:
    void setRenderMode(RenderMode);
    RenderMode renderMode();

  private:
    RenderMode renderMode_ = RenderMode::TEXCOORD;
  };
}


#endif /* OMNI_RENDEROPTIONS_H_ */

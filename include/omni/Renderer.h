#ifndef OMNI_RENDERER_H_
#define OMNI_RENDERER_H_

#include <QImage>
#include <omni/RenderOptions.h>
#include <omni/RenderBuffer.h>

namespace omni
{
  class Session;
  namespace proj
  {
    class Tuning;
  }

  class Renderer
  {
  public:
    Renderer(Session& _session, RenderOptions const& _options);
    
    RenderOptions const& options() const; 
    RenderOptions& options(); 
    void setOptions(RenderOptions const&);

    void renderToFile(QString const& _filename);
    
    
    void render(proj::Tuning const* _tuning, RenderBuffer& _buffer);
    void render(proj::Tuning const* _tuning, QImage& _image);



  private:
    void getUpper8bit(RenderBuffer const& _buffer, QImage& _image);
    void getLower8bit(RenderBuffer const& _buffer, QImage& _image);
    void getAlphaMask(RenderBuffer const& _buffer, QImage& _image);

    void bufferToRGBImage(RenderBuffer const& _buffer, QImage& _image);

    template<typename PROJECTION, typename MODELVIEW>
    static void renderToBuffer(RenderBuffer& _buffer, PROJECTION _proj, MODELVIEW _mv);

    template<typename OPERATION>
    static void bufferToImage(RenderBuffer const& _buffer, QImage& _image, OPERATION _f);

    Session& session_;
    RenderOptions options_;
  };
}

#endif /* OMNI_RENDERER_H_ */

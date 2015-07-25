#ifndef OMNI_VISUAL_TUNING_H_
#define OMNI_VISUAL_TUNING_H_

#include <QRectF>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <omni/proj/Tuning.h>

namespace omni
{
  namespace visual
  {
    class Tuning 
    {
    public:
      Tuning(omni::proj::Tuning&);

      omni::proj::Tuning const& tuning() const;

      void update();

      void updateBlendTexture();
      
      void drawWarpGrid() const;

      void drawBlendMask() const;

      /**@brief Calculates the view rectangle for orthographic 2D projection 
       **/
      QRectF viewRect(int _width, int _height, bool _keepAspectRatio) const;

    private:
      omni::proj::Tuning& tuning_;

      static std::unique_ptr<QOpenGLShaderProgram> blendShader_;
      std::unique_ptr<QOpenGLTexture> blendTex_;
    };
  }
}

#endif /* OMNI_VISUAL_TUNING_H_ */

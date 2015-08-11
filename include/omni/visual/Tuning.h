#ifndef OMNI_VISUAL_TUNING_H_
#define OMNI_VISUAL_TUNING_H_

#include <QRectF>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <omni/proj/Tuning.h>
#include <omni/visual/WarpGrid.h>

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
      void setBlendTextureUpdateRect(QRect const&);
      
      /// Draw Test card image for screen setup
      void drawTestCard(int _id) const;

      /**@brief Draws warp grid with lines and handles
         @param _rect Screen Rectangle of input
       **/
      void drawWarpGrid() const;

      /// Draw warp grip without lines and handles
      void drawWarpPatch() const;

      /// Draws Blendmask with warp grid and brush stroke layer 
      void drawBlendMask() const;


      /// Draw cursor for blend brush
      void drawCursor(QPointF const& _pos);

      bool initialized() const;

      void free();

    private: 

      /**@brief Calculates rectangle of this tuning  
       **/
      QRectF tuningRect() const;

      omni::proj::Tuning& tuning_;

      std::unique_ptr<visual::WarpGrid> warpGrid_;

      static std::unique_ptr<QOpenGLShaderProgram> blendShader_;
      std::unique_ptr<QOpenGLTexture> blendTex_;

      static std::unique_ptr<QOpenGLShaderProgram> testCardShader_;
      QRect blendTextureUpdateRect_;
    };
  }
}

#endif /* OMNI_VISUAL_TUNING_H_ */

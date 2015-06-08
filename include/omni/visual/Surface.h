#ifndef OMNI_VISUAL_SURFACE_H_
#define OMNI_VISUAL_SURFACE_H_

#include <QRectF>
#include <omni/visual/Cursor.h>

namespace omni
{
  namespace visual
  {
    /**@brief visual::Surface handles painting on a orthogonal 2D canvas
      *@detail 
     **/
    class Surface : public Interface
    {
    public:

      QSize sceneSize() const;
      void setSceneSize(QSize);
      qreal sceneAspect() const;
      QRectF sceneRect(bool _upsideDown = false) const;

      QSize windowSize() const;
      void setWindowSize(QSize);
      qreal windowAspect() const;
      QRectF windowRect(bool _upsideDown) const;

    private:
      bool ignoreAspect_ = false;
      float border_ = 0.0;
      QSize windowSize_;
      QSize sceneSize_;
      Cursor cursor_;
    };
  }
}


#endif /* OMNI_VISUAL_SURFACE_H_ */

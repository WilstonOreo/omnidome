#ifndef OMNI_UI_MAPPINGGL_H_
#define OMNI_UI_MAPPINGGL_H_

#include "GLView.h"


namespace omni
{
  class Session;
  class Mapping;

  namespace ui
  {
    class MappingGL : public GLView
    {
      Q_OBJECT
    public:
      explicit MappingGL(QWidget *parent = nullptr);
      ~MappingGL();
      
      typedef Texture<Image4ub,GL_TEXTURE_2D> proj_texture_type;
      typedef Texture<Image8ub,GL_TEXTURE_2D> blend_texture_type;

      void initialize(Session* _session, size_t _mapId, const view_set_type& _views = view_set_type());
      void free();

      void initializeFrameBuffer();
      void useTexture();

      void interactive(bool _interactive);
      void showCursor(bool _showCursor);
 
      TBD_PROPERTY_REF(float,border)
      TBD_PROPERTY_REF(bool,ignoreAspect)
      TBD_PROPERTY_RO(bool,interactive)
      TBD_PROPERTY_RO(bool,showCursor)
    protected:
      void mouseMoveEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);
      void keyPressEvent(QKeyEvent* even);
      void initializeGL();
      void resizeGL(int _w, int _h);
      void paintGL();

   private:
      QRectF viewRect(bool _upsideDown = false) const;
      QRectF projRect() const;
      QPointF screenPos() const;
      QPointF toScreenPos(QPointF const&) const;
      Point2 pixelPos(Point2 const&) const;

      void drawWarpGrid() const;
      void drawScreenRect() const;
      void drawWarpPatch() const;
      void drawPositioning() const;

      void drawCursor() const;
      void drawBlendMask() const;

      void drawFrameBuffer() const;

      float drawLine(Point2 const&, Point2 const&, float _leftOver); 
      void drawBrush(Point2 const&);

      bool isPressed_;

      bool previewUpdated_;

      std::unique_ptr<proj_texture_type> projTex_;

      std::unique_ptr<blend_texture_type> strokeBlendTex_;

      Mapping const* mapping() const;
      Mapping* mapping();
      void drawPreview();

      std::unique_ptr<FrameBufferTexture> frameBuffer_;
      size_t mapId_;
      Scalar leftOverDistance_;
    };
  }
}

#endif /* OMNI_UI_MAPPINGGL_H_ */

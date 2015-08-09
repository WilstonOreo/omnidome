#ifndef OMNI_UI_TUNINGGLVIEW_H_
#define OMNI_UI_TUNINGGLVIEW_H_

#include <QOpenGLFramebufferObject> 

#include <omni/ui/GLView.h>
#include <omni/visual/Tuning.h>

namespace omni
{
  namespace ui
  {
    class TuningGLView : public GLView
    {
      Q_OBJECT
    public: 
      TuningGLView(QWidget* = nullptr);
      ~TuningGLView();

      /// If disabled, this widget accepts user inputs 
      bool viewOnly() const;

      /// If disabled, screen rect will be stretched over whole widget
      bool keepAspectRatio() const;

      /// Flag for toggling drawing. Only black background is shown when false
      bool isDrawingEnabled() const;

      /// Returns true if cursor is visible
      bool showCursor() const;

      /// Return relative border value
      float border() const;

      void setChildViews(std::set<TuningGLView*> const&);
      std::set<TuningGLView*> childViews() const;

      void updateContext(bool);
    
    public slots:
      /// Set tuning index from session 
      void setTuningIndex(int);

      void destroy();

      /// If disabled, screen rect will be stretched over whole widget
      void setKeepAspectRatio(bool);

      /// If true, this widget does NOT accept user inputs 
      void setViewOnly(bool);

      void setShowCursor(bool);

      /**@brief Set flag which tells if projector view is actually drawn 
       * @detail Used for activate/deactivate fullscreen view
      **/
      void setDrawingEnabled(bool);

      /// Set relative border distance
      void setBorder(float);

      /// Show different content for different session modes
      void sessionModeChange();

      void updateWithChildViews(bool _updateContext = true);

    protected:
      void paintGL();
      void mouseMoveEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent* event);
      void keyPressEvent(QKeyEvent* event);

    private:
      omni::proj::Tuning* tuning();
      omni::proj::Tuning const* tuning() const;

      /// Draw Canvas from Projector's perspective
      void drawCanvas();

      /// Draw warp grid with handles while keeping aspect ratio
      void drawWarpGrid();

      /// Draw blend mask with stroke buffer
      void drawBlendMask();

      /// Draw test card image
      void drawTestCard();

      /// Draw screen border (only if widget is not in view only mode)
      void drawScreenBorder();

      /// Update warp buffer which contains image of projector perspective
      void updateWarpBuffer();

      /// Get rectangle of orthogonal view frustum
      QRectF viewRect() const;

      /// Transform widget position to view position
      QPointF screenPos(QPointF const& _pos) const;

      // Transform widget position to position in tuning image
      QPointF pixelPos(QPointF const& _pos) const;
 
      /// Drawing function for drawing on orthogonal 2D surface within view rect
      template<typename F> 
      void drawOnSurface(F f);

      /// Initialize OpenGL objects
      bool initialize();

      /// Tuning index
      int index_ = -1;

      /// Flags which determines if aspect ratio is used when drawing content
      bool keepAspectRatio_ = false;

      /// Flag determines if user input is not accepted
      bool viewOnly_ = false;

      /// Flag for toggling drawing. Only black background is shown when false
      bool drawingEnabled_ = true;

      /// Flag which tells if mouse button is down
      bool mouseDown_ = false;

      /// Cursor position (is not mouse position when widget is view only)
      QPointF cursorPosition_;

      /// Show cursor flag (cursor is also shown when widget is view only) 
      bool showCursor_ = true;

      /// Flag which is set to true before this widget is destroyed
      bool aboutToBeDestroyed_ = false;
      
      /// Left over distance value for blend blush
      float leftOverDistance_ = 0.0;

      /// Relative border
      float border_ = 0.0; 

      std::shared_ptr<visual::Tuning> tuning_;
      
      /// Used for drawing grayscale
      std::unique_ptr<QOpenGLFramebufferObject> frameBuffer_;
      
      /// Frame buffer which holds a texture with current view image
      std::unique_ptr<QOpenGLFramebufferObject> warpGridBuffer_;
    
      /// Shader for displaying disabled output in grayscale
      std::unique_ptr<QOpenGLShaderProgram> grayscaleShader_;

      std::set<TuningGLView*> childViews_;
    };
  }
}

#endif /* OMNI_UI_TUNINGGLVIEW_H_ */

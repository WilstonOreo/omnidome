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

      /// Returns true if widget is in fullscreen mode
      bool isFullscreen() const;

      /// Return relative border value
      float border() const;
    
    public slots:
      /// Set tuning index from session 
      void setTuningIndex(int);

      /// If disabled, screen rect will be stretched over whole widget
      void setKeepAspectRatio(bool);

      /// If enable, this widget does not accept user inputs 
      void setViewOnly(bool);

      /**@brief Set flag which tells if projector view is actually drawn 
       * @detail Used for activate/deactivate fullscreen view
      **/
      void setDrawingEnabled(bool);

      /// Set relative border distance
      void setBorder(float);

      /// Show different content for different session modes
      void sessionModeChange();

      /**@brief Enable fullscreen mode with given rectangle, cannot be reverted!
         @detail Only works if this view holds a valid tuning
       **/
      void setFullscreen(Screen const& _screen);

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

      QRectF viewRect() const;
      
      bool initialize();

      /// Tuning index
      int index_ = -1;

      /// Flags which determines if aspect ratio is used when drawing content
      bool keepAspectRatio_ = false;

      /// Flag determines if user input is not accepted
      bool viewOnly_ = false;

      /// Flag for toggling drawing. Only black background is shown when false
      bool drawingEnabled_ = true;

      /// Flag which tells if fullscreen is enabled
      bool fullscreen_ = false;

      /// Relative border
      float border_ = 0.0; 

      std::unique_ptr<visual::Tuning> tuning_;

      /// Used for drawing grayscale
      std::unique_ptr<QOpenGLFramebufferObject> frameBuffer_;
    
      /// Shader for displaying disabled output in grayscale
      std::unique_ptr<QOpenGLShaderProgram> grayscaleShader_;
    };
  }
}

#endif /* OMNI_UI_TUNINGGLVIEW_H_ */

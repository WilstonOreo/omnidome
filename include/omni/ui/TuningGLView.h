#ifndef OMNI_UI_TUNINGGLVIEW_H_
#define OMNI_UI_TUNINGGLVIEW_H_

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

      /// Return relative border value
      float border() const;
    
    public slots:
      /// Set tuning index from session 
      void setTuningIndex(int);

      /// If disabled, screen rect will be stretched over whole widget
      void setKeepAspectRatio(bool);

      /// If enable, this widget does not accept user inputs 
      void setViewOnly(bool);
      
      /// Set relative border distance
      void setBorder(float);

      /// Show different content for different session modes
      void sessionModeChange();

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
      
      QRectF viewRect() const;
      
      bool initialize();

      /// Tuning index
      int index_ = -1;

      /// Flags which determines if aspect ratio is used when drawing content
      bool keepAspectRatio_ = false;

      /// Flag determines if user input is not accepted
      bool viewOnly_ = false;

      /// Relative border
      float border_ = 0.0; 

      std::unique_ptr<visual::Tuning> tuning_;
    };
  }
}

#endif /* OMNI_UI_TUNINGGLVIEW_H_ */

#ifndef OMNI_UI_INPUTPREVIEW_H_
#define OMNI_UI_INPUTPREVIEW_H_

#include <omni/ui/GLView.h>

namespace omni
{
  namespace ui 
  {
    class InputPreview : public GLView
    {
      Q_OBJECT
    public: 
      InputPreview(QWidget* = nullptr);
      ~InputPreview();

      /// Return relative border value
      float border() const;
    
    public slots:
      /// Set relative border distance
      void setBorder(float);

    protected:
      void paintGL();
      void mouseMoveEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);

    signals:
      void inputChanged();

    private:
      omni::proj::Tuning* tuning();
      omni::proj::Tuning const* tuning() const;
    
      QRectF viewRect() const;

      /// Set ruler position of input from widget coordinates
      void setRulerPos(QPointF const&);

      input::Interface* input();
      input::Interface const* input() const;

      /**@brief Transforms mouse position on widget to normalized position on input
       * @detail Position in range (0.0,0.0) and (1.0,1.0)
       **/
      QPointF screenPos(QPointF const& _pos) const;
      
      bool initialize();

      /// Relative border
      float border_ = 0.0; 
    };
  }
}

#endif /* OMNI_UI_INPUTPREVIEW_H_ */

#ifndef OMNI_UI_BRUSHPREVIEW_H_
#define OMNI_UI_BRUSHPREVIEW_H_

#include <memory>
#include <QWidget>
#include <omni/BlendBrush.h>

namespace omni
{  
  namespace ui
  {
    class BrushPreview : public QWidget
    {
      Q_OBJECT
    public:
      BrushPreview(QWidget* _parent = nullptr);
     
      void update(float _feather, bool _invert);

    protected:
      void paintEvent(QPaintEvent* event);

      void resizeEvent(QResizeEvent* event);
      
    private:
      BlendBrush brush_;
      QImage image_;

    };
  }
}

#endif /* OMNI_UI_BRUSHPREVIEW_H_ */

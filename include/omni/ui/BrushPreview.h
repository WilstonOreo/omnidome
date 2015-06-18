#ifndef OMNI_UI_BRUSHPREVIEW_H_
#define OMNI_UI_BRUSHPREVIEW_H_

#include <memory>
#include <QWidget>

namespace omni
{  
  namespace ui
  {
    class BrushPreview : public QWidget
    {
      Q_OBJECT
    public:
      BrushPreview(QWidget* _parent = nullptr);
      
    protected:
      void paintEvent(QPaintEvent* event);
      
    private:
      qreal feather_ = 1.0;
      bool invert_ = false;
      std::unique_ptr<QImage> image_;

    };
  }
}

#endif /* OMNI_UI_BRUSHPREVIEW_H_ */

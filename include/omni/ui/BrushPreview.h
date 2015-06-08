#ifndef OMNI_UI_BRUSHPREVIEW_H_
#define OMNI_UI_BRUSHPREVIEW_H_

#include <memory>
#include <QWidget>
#include <tbd/property.h>

namespace omni
{  
  namespace ui
  {
    class BrushPreview : public QWidget
    {
      Q_OBJECT
    public:
      BrushPreview(QWidget* _parent = nullptr);
      
      TBD_PROPERTY_REF(float,feather)
      TBD_PROPERTY_REF(bool,invert)
    protected:
      void paintEvent(QPaintEvent* event);
      std::unique_ptr<QImage> image_;
    };
  }
}

#endif /* OMNI_UI_BRUSHPREVIEW_H_ */

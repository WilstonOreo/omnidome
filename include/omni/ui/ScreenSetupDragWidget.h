#ifndef OMNI_UI_SCREENSETUPDRAGWIDGET_H_
#define OMNI_UI_SCREENSETUPDRAGWIDGET_H_

#include <QWidget>
#include <QColor>

namespace omni
{
  namespace ui
  {
    class ScreenSetupDragWidget : public QWidget
    {
      Q_OBJECT
    public:
      ScreenSetupDragWidget(QWidget* = nullptr);
      ~ScreenSetupDragWidget();

      QColor color() const;
      void setColor(QColor const&);

      void setRect(QRectF const&);

    protected:
      void paintEvent(QPaintEvent*);

    private:
      QColor color_;
    };
  }
}


#endif /* OMNI_UI_SCREENSETUPDRAGWIDGET_H_ */

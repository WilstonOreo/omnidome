#ifndef OMNI_UI_FULLSCREEN_H_
#define OMNI_UI_FULLSCREEN_H_

#include <QWidget>
#include <QScreen>
#include <omni/util.h>
#include <omni/ui/ScreenSetupDragWidget.h>

namespace omni
{
  namespace ui
  {
    /// Widget for fullscreen display
    class FullScreen : public QWidget 
    {
      Q_OBJECT
    public:
      FullScreen(const QScreen* _screen);
      ~FullScreen();

      void hideDragWidget();
      void showDragWidget(int _subIndex, QColor const& _color);

      void attach(int _subIndex, QWidget* _widget);
      void detach(int _subIndex);
      void detachAll();

    protected:
      void closeEvent(QCloseEvent* _event);

    private:

      void setGeometry(QWidget*, int _subIndex) const;

      QScreen const* screen_;
      std::map<int,QWidget*> widgets_;
      QUniquePtr<ScreenSetupDragWidget> dragWidget_;
    };
  }
}


#endif /* OMNI_UI_FULLSCREEN_H_ */

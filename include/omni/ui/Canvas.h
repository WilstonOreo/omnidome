#ifndef OMNI_UI_CANVAS_H_
#define OMNI_UI_CANVAS_H_

#include <memory>
#include <QGroupBox>
#include <omni/ui/SessionWidget.h>

namespace omni
{
  class Session;
  
  namespace ui
  {
    namespace Ui
    {
      class Canvas;
    }

    class Canvas : 
      public QGroupBox,
      public SessionWidget
    {
      Q_OBJECT
    public:
      Canvas(QWidget* = nullptr);
      ~Canvas();

    signals:
      void canvasTypeChanged();
      void canvasChanged();
      void displayInputToggled(bool);

    public slots:
      void canvasTypeSelected(QString const&);

      void sessionParameters();

    private:
      std::unique_ptr<Ui::Canvas> ui_;
    };
  }
}

#endif /* OMNI_UI_CANVAS_H_ */

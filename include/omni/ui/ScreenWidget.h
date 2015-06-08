#ifndef OMNI_UI_SCREENMANAGER_H_
#define OMNI_UI_SCREENMANAGER_H_

#include <QGraphicsWidget>
#include "ScreenItem.h"

namespace omni
{
  namespace ui
  {
    class Fullscreen;

    class ScreenWidget : public QGraphicsWidget
    {
      Q_OBJECT
    public:
      ScreenWidget(QWidget* = nullptr);
      ~ScreenWidget();

      void updateScreens();

      void showScreenIdentifiers();

    private:
      std::vector<std::unique_ptr<Screen>> screen_;
      std::vector<QUniquePtr<Fullscreen>> fullscreens_;
    };
  }
}


#endif /* OMNI_UI_SCREENWIDGET_H_ */


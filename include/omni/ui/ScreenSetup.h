#ifndef OMNI_UI_SCREENSETUP_H_
#define OMNI_UI_SCREENSETUP_H_

#include <list>
#include <QWidget>

#include <omni/proj/Tuning.h>

namespace omni
{
  namespace ui
  {
    class ScreenSetup : public QWidget
    {
      Q_OBJECT
    public:
      ScreenSetup(QWidget* = nullptr);
      ~ScreenSetup();

    public slots:
      void updateScreens();

    protected:
      void paintEvent(QPaintEvent*);

    private:
      class Item
      {
      public:
        Item(ScreenSetup&,Screen const&);

        void paint(QPainter*);

        /// Attach projector to screen
        void attachTuning(omni::proj::Tuning*);

        /// Detach projector from item
        void detachTuning();
        omni::proj::Tuning const* tuning() const;

        QRectF boundingRect() const;

      private:
        ScreenSetup& screenSetup_;
        Screen const& screen_;
        omni::proj::Tuning* tuning_ = nullptr;
      };

      std::list<Item> screenItems_;
      QRectF desktopRect_;
    };

  }
}

#endif /* OMNI_UI_SCREENSETUP_H_ */

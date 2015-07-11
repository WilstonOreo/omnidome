#ifndef OMNI_UI_SCREENSETUP_H_
#define OMNI_UI_SCREENSETUP_H_

#include <list>
#include <QWidget>

#include <omni/proj/Tuning.h>
#include <omni/proj/ScreenSetup.h>

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


      float zoom() const;

    public slots:
      void updateScreens();
      void setZoom(float);

    protected:
      void paintEvent(QPaintEvent*);

      void mouseMoveEvent(QMouseEvent*);

    private:
      friend class Item;

      class Item
      {
      public:
        Item(ScreenSetup&,Screen const&);

        void paint(QPainter&);

        /// Attach projector to screen
        void attachTuning(omni::proj::Tuning*);

        /// Detach projector from item
        void detachTuning();
        omni::proj::Tuning const* tuning() const;
  
        /// Returns flag whether
        bool mouseOver() const;
        void setMouseOver(bool);

        bool drop() const;
        void setDrop(bool);

        QRectF rect() const;

      private:
        bool mouseOver_ = false;
        bool drop_ = false;
        ScreenSetup& screenSetup_;
        Screen const& screen_;
        omni::proj::Tuning* tuning_ = nullptr;
      };

      /// Returns transformed bounding rect which fits into window and keeps aspect ratio
      QRectF transformedRect() const;

      /// Returns Transformed copy of the rect, based on transformed desktop rect
      QRectF transformedRect(QRectF const&) const;

      /// Returns the scaling factor which is needed so that desktopRect_ fits into window
      float scalingFactor() const;

      float zoom_ = 0.9;
      std::list<Item> screenItems_;
      QRectF desktopRect_;
      omni::proj::ScreenSetup setup_;
    };

  }
}

#endif /* OMNI_UI_SCREENSETUP_H_ */

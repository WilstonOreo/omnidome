#ifndef OMNI_UI_SCREENSETUP_H_
#define OMNI_UI_SCREENSETUP_H_

#include <list>
#include <QWidget>

#include <omni/ui/proj/Tuning.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/ui/ScreenSetupDragWidget.h>

namespace omni
{
  class Session;
 
  namespace ui
  {
    /**@brief A widget for setup fullscreen windows
     * @details Holds fullscreen widgets
     **/
    class ScreenSetup : public QWidget
    {
      Q_OBJECT
    public:
      ScreenSetup(QWidget* = nullptr);
      ~ScreenSetup();

      Session const* session() const;
      void setSession(Session*);

      float zoom() const;

    public slots:
      /// Update screen dimensions
      void updateScreens();

      void detachTuning(omni::ui::proj::Tuning*);

      /// Set zoom factor
      void setZoom(float);

    protected:
      void paintEvent(QPaintEvent*);

      void mouseMoveEvent(QMouseEvent*);
      
      void dragEnterEvent(QDragEnterEvent*);
      void dragMoveEvent(QDragMoveEvent*);
      void dropEvent(QDropEvent*);

    private:
      friend class Item;

      /// A screen rectangle item with drawing functionality
      class Item
      {
      public:
        Item(ScreenSetup&,Screen const&);

        void paint(QPainter&);
        
        omni::ui::proj::Tuning* tuning();
        omni::ui::proj::Tuning const* tuning() const;

        /// Attach projector to screen
        void attachTuning(omni::ui::proj::Tuning*);

        /// Detach projector from item
        void detachTuning();
          
        /// Returns flag whether mouse is currently over this item
        bool mouseOver() const;
        void setMouseOver(bool);

        /// Flag whether a drop is supposed to happen on this item 
        bool drop() const;
        void setDrop(bool, QColor const& _color = QColor("#FFFFFF"));

        QRectF rect() const;

      private:
        bool mouseOver_ = false;
        bool drop_ = false;
        QColor dropColor_;
        ScreenSetup& screenSetup_;
        Screen const& screen_;
        omni::ui::proj::Tuning* tuning_ = nullptr;
      };

      /// Returns transformed bounding rect which fits into window and keeps aspect ratio
      QRectF transformedRect() const;

      /// Returns Transformed copy of the rect, based on transformed desktop rect
      QRectF transformedRect(QRectF const&) const;

      /**@brief Returns pointer to a ScreenSetup::Item under given position
        *@detail Returns null otherwise
      **/
      Item* getItemAtPos(QPoint const&);

      /// Returns the scaling factor which is needed so that desktopRect_ fits into window
      float scalingFactor() const;

      float zoom_ = 0.9;
      std::list<Item> screenItems_;
      QRectF desktopRect_;
      omni::proj::ScreenSetup* setup_;
      Session* session_ = nullptr;
      QUniquePtr<ScreenSetupDragWidget> dragWidget_;
    };

  }
}

#endif /* OMNI_UI_SCREENSETUP_H_ */

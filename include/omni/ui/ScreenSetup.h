#ifndef OMNI_UI_SCREENSETUP_H_
#define OMNI_UI_SCREENSETUP_H_

#include <list>
#include <QWidget>

#include <omni/ui/proj/Tuning.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/ui/ScreenSetupDragWidget.h>
#include <omni/ui/FullScreen.h>

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

      static int subScreenCount(QScreen const*);
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

      friend class SubScreenItem;

      class Item;

      class SubScreenItem
      {
      public:
        SubScreenItem();
        SubScreenItem(Item const* _parent, int _index);
        
        omni::ui::proj::Tuning* tuning();
        omni::ui::proj::Tuning const* tuning() const;
        void setTuning(omni::ui::proj::Tuning* _tuning);
        
        QRect rect() const;
        QSize size() const;

        void paint(bool _hover, bool _drop, QColor _dropColor, QPainter&);

      private:
        Item const* parent_ = nullptr;
        int index_ = -1;
        omni::ui::proj::Tuning* tuning_ = nullptr;
      };

      /// A screen rectangle item with drawing functionality
      class Item
      {
      public:
        Item(ScreenSetup&,QScreen const* _screen);
        ~Item();

        QScreen const* screen() const;

        void paint(QPainter&);
 
        /// Attach projector to screen on current hover index
        void attachTuning(omni::ui::proj::Tuning*);

        /// Detach projector from item on current hover index
        void detachTuning();

        /// Returns flag whether mouse is currently over this item
        int hoverIndex() const;
        void setHoverIndex(int);

        /// Set hover index from mouse position
        void setHoverIndex(QPoint const&);

        /// Flag whether a drop is supposed to happen on this item 
        bool drop() const;
        void setDrop(bool, QColor const& _color = QColor("#FFFFFF"));

        /// Hides fullscreen widget if no tunings are attached
        void endDrop();

        QRect rect() const;

      private:
        int hoverIndex_ = -1;
        bool drop_ = false;
        QColor dropColor_;
        ScreenSetup& screenSetup_;
        QScreen const* screen_;
        FullScreen* fullscreen_;
        std::vector<SubScreenItem> subScreens_;
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
      std::map<QScreen const*,std::unique_ptr<Item>> screenItems_;
      QRectF desktopRect_;
      omni::proj::ScreenSetup* setup_;
      Session* session_ = nullptr;
    };

  }
}

#endif /* OMNI_UI_SCREENSETUP_H_ */

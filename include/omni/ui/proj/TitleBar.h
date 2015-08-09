#ifndef OMNI_UI_PROJ_TITLEBAR_H
#define OMNI_UI_PROJ_TITLEBAR_H

#include <QToolButton>
#include <QMenu>
#include <slim/Widget.h>
#include <slim/mixin/RangedValue.hpp>
#include <omni/util.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      class Tuning;

      /**@brief TitleBar for projector tuning widget
       * @detail Has four buttons:
       *         * projector setup selection and resetting
       *         * Fullscreen display toggle options
       *         * Minimizing/Maximizing tuning widget
       *         * Close tuning widget for deleting a projector tuning
       **/
      class TitleBar :
        public slim::Widget
      {
        Q_OBJECT
      public:
        /// Construct with tuning widget as parent (is mandatory)
        TitleBar(Tuning*);
        
        /// Construct with label and tuning widget as parent
        TitleBar(QString const& _label, Tuning*);
        ~TitleBar();

        /// Return color
        QColor color() const;

        /// Returns true if close button is visible
        bool closeButtonVisible() const;

      public slots:
        /// Set color from gradient pos (between 0.0 and 1.0)
        void setColor(QColor const&);
        
        /// Select color from color picker
        void selectColor();

        /// Toggle visibility of close button
        void setCloseButtonVisible(bool);

      signals:
        void closeButtonClicked();
        void maximizeButtonClicked();
        void colorChanged(QColor const&);

        void freeSetupSelected();
        void peripheralSetupSelected();

      protected:
        void paintEvent(QPaintEvent*);

      private:
        void setup();

        /// Set moving flag to value and handle visibility of buttons 
        Tuning* tuningWidget();
         
        /// Button for resetting and projector setup selection
        QUniquePtr<QToolButton> menuButton_;
        
        /// Button for toggling fullscreen display
        QUniquePtr<QToolButton> displayButton_;
       
        /// Button for Minimizing/Maximizing widget
        QUniquePtr<QToolButton> maximizeButton_;
        
        /// Button for closing widget
        QUniquePtr<QToolButton> closeButton_;

        /// Context menu widget
        QUniquePtr<QMenu> menu_;

        /// Parent tuning widget
        Tuning* tuningWidget_ = nullptr;

        QColor color_;
      };
    };
  }
}

#endif /* OMNI_UI_PROJ_TITLEBAR_H */

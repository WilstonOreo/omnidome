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
        public slim::Widget,
        private slim::mixin::RangedValue<TitleBar,double>
      {
        Q_OBJECT
      public:
        typedef slim::mixin::RangedValue<TitleBar,double> mixin_range_type;
        using mixin_range_type::value;

        /// Construct with tuning widget as parent (is mandatory)
        TitleBar(Tuning*);
        
        /// Construct with label and tuning widget as parent
        TitleBar(QString const& _label, Tuning*);
        ~TitleBar();

        /// Return gradient color
        QColor color() const;
       
        /// Returns movement flag
        bool isMoving() const;

      public slots:
        /// Set value (must between 0.0 and 1.0)
        //
        void setValue(double);
      signals:
        
        void valueChanged();
        void closeButtonClicked();
        void maximizeButtonClicked();
        void displayButtonClicked(bool);

      protected:
        void paintEvent(QPaintEvent*);
        void mouseMoveEvent(QMouseEvent*);
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent*);

      private:
        void setup();

        /// Set moving flag to value and handle visibility of buttons 
        void setMoving(bool _moving);
        Tuning* tuningWidget();

        /// Calculate position from value
        double valueToPos() const;

        /// Calculate position from value
        double valueToPos(double) const;

        /// Get value from x mouse position
        double valueFromPos(double) const;
        
        /// Moving flag
        bool moving_ = false;

        /// Spectral gradient 
        QLinearGradient gradient_;
        
        /// Button for resetting and projector setup selection
        QUniquePtr<QToolButton> menuButton_;
        
        /// Button for toggling fullscreen display
        QUniquePtr<QToolButton> displayButton_;
       
        /// Button for Minimizing/Maximizing widget
        QUniquePtr<QToolButton> maximizeButton_;
        
        /// Button for closing widget
        QUniquePtr<QToolButton> closeButton_;

        QUniquePtr<QMenu> menu_;

        /// Parent tuning widget
        Tuning* tuningWidget_ = nullptr;
      };
    };
  }
}

#endif /* OMNI_UI_PROJ_TITLEBAR_H */

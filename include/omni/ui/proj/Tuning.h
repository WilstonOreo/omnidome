#ifndef OMNI_UI_PROJ_TUNING_H
#define OMNI_UI_PROJ_TUNING_H

#include <omni/ui/ParameterWidget.h>
#include <omni/proj/Tuning.h>

namespace omni
{
  class Session;

  namespace proj
  {
    class Tuning;
  }

  namespace ui
  {
    class TuningGLView;

    namespace proj
    {
      class TitleBar;

      /**@brief Widget for manipulating projector parameters
         @detail Also holds a preview OpenGL widget
       **/
      class Tuning : public ParameterWidget
      {
        Q_OBJECT
      public:
        /// View mode (determines which elements are to be displayed)
        enum WindowState
        {
          NO_DISPLAY, // Widget is minimized
          DISPLAY_ONLY, // Only preview is displayed
          FOV_SLIDERS, // Only FOV sliders are displayed
          ADJUSTMENT_SLIDERS, // Only sliders for adjustment are displayed
          NUM_WINDOW_STATES
        };

        /// Default constructor 
        Tuning(QWidget* _parent = nullptr);
        
        /// Constructs with a given tuning (called by default from TuningList)
        Tuning(int _index, omni::Session* _session, QWidget* _parent = nullptr);
        
        /// Destructor
        ~Tuning();
   
        /// Return tuning
        omni::proj::Tuning* tuning();
        
        /// Return tuning (const version)
        omni::proj::Tuning const* tuning() const;
        
        /// Set tuning from session and index
        void setTuning(int _index, omni::Session*);
    
        /// Return index of tuning
        int index() const;

        Session const* session() const;

        /// Return selected flag
        bool isSelected() const;

        /// Return current window state
        WindowState windowState() const;

      public slots:
        /// Sets Window State which determines which elements are visible
        void setWindowState(WindowState _mode);
        
        /// Selects succeeding window state
        void setNextWindowState();

        /**@brief Sets flag if Tuning Widget is active
           @detail A tuning widget should only be active when 
         **/
        void setSelected(bool);
        
        /// Reorders widgets according to given window state
        void reorderWidgets();

        /// Display content and widget for specific session mode
        void sessionModeChange();

      signals:
        void selected();
        void closed(int);
        void projectorSetupChanged();

      protected:
        /// Handles resizing of sliders and preview 
        void resizeEvent(QResizeEvent*);

        /// Handles proper resizing of the widget
        void showEvent(QShowEvent*);

        /// Paint border
        void paintEvent(QPaintEvent*);

        /// Mouse Move Event and handler for dragging to ScreenSetup widget
        void mouseMoveEvent(QMouseEvent*);

        /// Handles focus events from child widgets
        bool eventFilter(QObject*,QEvent*);

        /// Focus in event used by TuningList to set current tuning for session
        void focusInEvent(QFocusEvent*);
        
        /// Focus out for deselecting tuning
        void focusOutEvent(QFocusEvent*);

      private slots:

        void startDrag();

        /// Set parameters from sliders to tuning
        void updateParameters();
        
        /// Update slider and border color
        void updateColor();
        
        /// Clean up. 
        void prepareRemove();

      private:
        /// Setup (only called in constructor)
        void setup();
 
        /// The index of the tuning
        int index_ = -1;

        // The associated session
        omni::Session* session_ = nullptr; 

        /// Slider parameter widgets, grouped
        std::map<QString,std::vector<QWidget*>> sliderGroups_;
         
        /// Title bar widget
        TitleBar* titleBar_ = nullptr;
        
        /// GL preview widget
        TuningGLView* glView_ = nullptr;
      
        /// Window State
        WindowState windowState_ = ADJUSTMENT_SLIDERS;

        /// isSelected_ flag (should be true when widget is currently selected) 
        bool isSelected_ = true;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNING_H */

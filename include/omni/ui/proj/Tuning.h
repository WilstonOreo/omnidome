#ifndef OMNI_UI_PROJ_TUNING_H
#define OMNI_UI_PROJ_TUNING_H

#include <omni/proj/Tuning.h>
#include <omni/ui/ParameterWidget.h>
#include <omni/ui/proj/TuningLayout.h>

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
      class TuningLayout;

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

        typedef TuningLayout::widgetgroup_type widgetgroup_type;

        /// Default constructor 
        Tuning(QWidget* _parent = nullptr);
        
        /// Constructs with a given tuning (called by default from TuningList)
        Tuning(int _index, omni::Session* _session, QWidget* _parent = nullptr);
        
        /// Destructor
        virtual ~Tuning();
   
        /// Return tuning
        omni::proj::Tuning* tuning();
        
        /// Return tuning (const version)
        omni::proj::Tuning const* tuning() const;

        /// Set tuning from session and index
        void setTuning(int _index, omni::Session*);
    
        TuningGLView* fullscreenWidget();
        TuningGLView const* fullscreenWidget() const;
        
        TuningGLView* previewWidget();
        TuningGLView const* previewWidget() const;

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
        
        /// Display content and widget for specific session mode
        void sessionModeChange();
 
        /// Attaches a screen to this tuning
        void attachScreen(QScreen const* _screen);

        /**@brief Detaches screen from this tuning
           @detail Also triggered when QGuiApplication::screenAdded or 
                   QGuiApplication::screenRemoved were fired
         **/
        void detachScreen();

        /// Update GL View and Fullscreen view, if there is any
        void updateViews();
        
        /// Enable or disable fullscreen display
        void fullscreenToggle(bool);

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
        
        /// Set FOV to projector from slider
        void setFov();

        /// Set Throw Ratio to projector from slider
        void setThrowRatio();

        /// Update slider and border color
        void updateColor();
        
        /// Clean up. 
        void prepareRemove();

      private:
        /// Setup (only called in constructor)
        void setup();
 
        /// The index of the tuning
        int index_ = -1;

        /// Adds a new/changes a parameter group
        void addGroup(QString const& _groupName, widgetgroup_type const& _widgets);

        void setGroup(QString const& _groupName);

        // The associated session
        omni::Session* session_ = nullptr; 
 
        /// Title bar widget
        TitleBar* titleBar_ = nullptr;
        
        /// GL preview widget
        TuningGLView* glView_ = nullptr;
 
        // Fullscreen GL widget
        QUniquePtr<TuningGLView> fullscreen_;

        /// Window State
        WindowState windowState_ = ADJUSTMENT_SLIDERS;

        /// Is true when this widgets index and current tuning index are equal 
        bool isSelected_ = true;

        /// Layout
        TuningLayout* layout_ = nullptr;
 
        std::map<QString,widgetgroup_type> groups_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNING_H */

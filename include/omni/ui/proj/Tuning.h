#ifndef OMNI_UI_PROJ_TUNING_H
#define OMNI_UI_PROJ_TUNING_H

#include <set>
#include <QWidget>
#include <slim/RangedFloat.h>
#include <omni/proj/Tuning.h>

namespace omni
{
  namespace proj
  {
    class Tuning;
  }

  namespace ui
  {
    class GLView2D;

    namespace proj
    {
      class TitleBar;

      /**@brief Widget for manipulating projector parameters
         @detail Also holds a preview OpenGL widget
       **/
      class Tuning : public QWidget
      {
        Q_OBJECT
      public:
        /// View mode (determines which elements are to be displayed)
        enum ViewMode
        {
          NO_DISPLAY, // Widget is minimized
          DISPLAY_ONLY, // Only preview is displayed
          FOV_SLIDERS, // Only FOV sliders are displayed
          ADJUSTMENT_SLIDERS, // Only sliders for adjustment are displayed
          NUM_MODES
        };

        /// Default constructor 
        Tuning(QWidget* _parent = nullptr);
        
        /// Constructs with a given tuning (called by default from TuningList)
        Tuning(omni::proj::Tuning* _tuning, QWidget* _parent = nullptr);
        
        /// Destructor
        ~Tuning();

        /// Returns current 
        ViewMode mode() const;
        
        /// Return tuning
        omni::proj::Tuning* tuning();
        
        /// Return tuning (const version)
        omni::proj::Tuning const* tuning() const;
        
        /// Set tuning
        void setTuning(omni::proj::Tuning*);

      public slots:
        /// Sets ViewMode
        void setViewMode(ViewMode _mode);
        
        /// Selects succeeding view mode
        void setNextViewMode();

        /**@brief Sets flag if Tuning Widget is active
           @detail A tuning widget should only be active when 
         **/
        void setActive(bool);
        
        /// Reorders widgets according to given view mode
        void reorderWidgets();


      protected:
        /// Handles resizing of sliders and preview 
        void resizeEvent(QResizeEvent*);

        /// Handles proper resizing of the widget
        void showEvent(QShowEvent*);

        /// Paint border
        void paintEvent(QPaintEvent*);
        
        /// Handles focus events from child widgets
        bool eventFilter(QObject*,QEvent*);

      private slots:

        /// Set parameters from sliders to tuning
        void updateParameters();
        
        /// Update slider and border color
        void updateColor();

      private:
        /// Setup (only called in constructor)
        void setup();

        /// Adds a widget with a name, a value and min-max range
        slim::RangedFloat* addWidget(QString const&,float,float,float);
        
        /// Adds an angle widget with a name, a value and min-max range
        slim::RangedFloat* addAngleWidget(QString const&,float,float,float);
        
        /// Adds an offset widget with a name, a value and min-max range
        slim::RangedFloat* addOffsetWidget(QString const&,float,float,float);

        /// The associated tuning 
        omni::proj::Tuning* tuning_ = nullptr;
      
        /// Slider parameter widgets
        std::vector<QWidget*> parameters_;
        
        /// Slider parameter widgets, grouped
        std::map<QString,std::vector<QWidget*>> sliderGroups_;
        
        /// Slider widgets, sorted by their label
        std::map<QString,QWidget*> parameterMap_;
      
        /// Title bar widget
        TitleBar* titleBar_ = nullptr;
        
        /// GL preview widget
        GLView2D* glView_ = nullptr;
      
        /// View mode
        ViewMode mode_ = ADJUSTMENT_SLIDERS;

        /// Active flag (should be true when widget is currently selected) 
        bool active_ = true;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNING_H */

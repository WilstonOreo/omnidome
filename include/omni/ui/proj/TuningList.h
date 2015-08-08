#ifndef OMNI_UI_PROJ_TUNINGLIST_H
#define OMNI_UI_PROJ_TUNINGLIST_H

#include <memory>
#include <QScrollArea>
#include <omni/util.h>
#include <omni/ui/proj/Tuning.h>

namespace omni
{
  class Session;

  namespace ui
  {
    namespace proj
    {
      class Tuning;

      class TuningList : public QScrollArea
      {
        Q_OBJECT
      public:
        // Maximum number of tunings one can add
        inline static constexpr int maxNumberTunings() { return 16; }

        TuningList(QWidget* = nullptr);
        ~TuningList();

        /// The session
        Session const* session() const;
        
        /// Set new session and update subwidgets
        void setSession(Session*);

      public slots:
        /// Change current mode for all tuning widgets
        void sessionModeChange();

        /// Add a new tuning and tuning widget, with automatically detected settings
        void addTuning();

        /// Remove tuning 
        void removeTuning(int _index);
        
        /// Set current tuning from currently selected widget 
        void setCurrentTuning();

        /// Set current tuning index manually (currentIndexChanged signal is emitted)
        void setTuningIndex(int); 

        /// Remove all mappings from session and all associated widgets
        void clear();

        /// Updates/Repaints GL Views of all tunings widgets
        void updateViews();

      protected:
        void resizeEvent(QResizeEvent*);

      signals:
        /// Signal which is emitted when the current tuning has changed
        void currentIndexChanged(int);

        /// Signal which is emitted when parameters of one tuning have changed
        void projectorSetupChanged();

      private:
        /// Add widget from existing tuning
        void addTuning(omni::proj::Tuning* _tuning);


        /**@brief Removes all widgets in the scroll area
          *@detail Unlike clear() it does not remove tunings from session
        **/
        void removeWidgets();

        /// Get most differing color for a new tuning
        QColor getTuningColor();

        QWidget* contents_;
        QLayout* layout_;

        Session* session_ = nullptr;
        std::vector<QUniquePtr<Tuning>> widgets_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLIST_H */

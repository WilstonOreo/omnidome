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
        TuningList(QWidget* = nullptr);
        ~TuningList();

        Session const* session() const;
        void setSession(Session*);

      public slots:
        /// Change current mode for all tuning widgets
        void sessionModeChange();

        /// Add a new tuning and tuning widget
        void addTuning();

        /// Remove tuning 
        void removeTuning(int _index);
        
        /// Set current tuning from currently selected widget 
        void setCurrentTuning();

        /// Remove all mappings from session and all associated widgets
        void clear();

      protected:
        void resizeEvent(QResizeEvent*);

      signals:
        void currentIndexChanged(int);
        void projectorSetupChanged();

      private:
        QWidget* contents_;
        QLayout* layout_;

        Session* session_ = nullptr;
        std::vector<QUniquePtr<Tuning>> widgets_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLIST_H */

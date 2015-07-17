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
        void addTuning();
        void removeTuning(int _index);
        void setCurrentTuning();

        /// Set view mode for all tuning widgets
        void setViewMode(Tuning::ViewMode);

        /// Remove all mappings from session and all associated widgets
        void clear();

      protected:
        void resizeEvent(QResizeEvent*);

      signals:
        void projectorSetupChanged();

      private:

        QWidget* contents_;
        QLayout* layout_;

        Session* session_ = nullptr;
        std::vector<QUniquePtr<Tuning>> widgets_;
        Tuning::ViewMode viewMode_ = Tuning::FOV_SLIDERS;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLIST_H */

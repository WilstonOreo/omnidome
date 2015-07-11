#ifndef OMNI_UI_PROJ_TUNINGLIST_H
#define OMNI_UI_PROJ_TUNINGLIST_H

#include <memory>
#include <QScrollArea>
#include <omni/util.h>

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
        void removeTuning();
        void setCurrentTuning();

        /// Removes all mappings from session and all associated widgets
        void clear();

      protected:
        void resizeEvent(QResizeEvent*);

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

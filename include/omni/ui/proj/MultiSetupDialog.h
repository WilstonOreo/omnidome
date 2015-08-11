#ifndef OMNI_UI_PROJ_MULTISETUPDIALOG_H_
#define OMNI_UI_PROJ_MULTISETUPDIALOG_H_

#include <QDialog>
#include <omni/proj/MultiSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      namespace Ui
      {
        class MultiSetupDialog;
      };

      class MultiSetupDialog : public QDialog
      {
        Q_OBJECT
      public:
        enum class Result
        {
          CANCELLED,
          APPEND,
          REPLACE
        };

        MultiSetupDialog(omni::proj::MultiSetup* _multiSetup, Session* _session);
        ~MultiSetupDialog();

        static Result open(omni::proj::MultiSetup* _multiSetup, Session* _session);

      public slots:
        void replace();
        void append();
        void cancel();

      private:
        Session* session_ = nullptr;
        omni::proj::MultiSetup* multiSetup_ = nullptr;
        std::unique_ptr<Ui::MultiSetupDialog> ui_;
        Result result_ = Result::CANCELLED;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUPDIALOG_H_ */

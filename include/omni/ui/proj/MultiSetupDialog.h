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
        MultiSetupDialog(QString const& _templateName, Session* _session);
        ~MultiSetupDialog();

      public slots:
        void replace();
        void append();

      signals:

        void appendClicked();
        void replaceClicked();

      private:
        Session* session_ = nullptr;
        std::unique_ptr<omni::proj::MultiSetup> template_;
        std::unique_ptr<Ui::MultiSetupDialog> ui_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUPDIALOG_H_ */

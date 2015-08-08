#ifndef OMNI_UI_PROJ_TEMPLATEDIALOG_H_
#define OMNI_UI_PROJ_TEMPLATEDIALOG_H_

#include <QDialog>
#include <omni/proj/Template.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      namespace Ui
      {
        class TemplateDialog;
      };

      class TemplateDialog : public QDialog
      {
        Q_OBJECT
      public:
        TemplateDialog(QString const& _templateName, Session* _session);
        ~TemplateDialog();

      public slots:
        void replace();
        void append();

      signals:

        void appendClicked();
        void replaceClicked();

      private:
        Session* session_ = nullptr;
        std::unique_ptr<omni::proj::Template> template_;
        std::unique_ptr<Ui::TemplateDialog> ui_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TEMPLATEDIALOG_H_ */

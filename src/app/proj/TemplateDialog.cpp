#include <omni/ui/proj/TemplateDialog.h>

#include "ui_omni_ui_TemplateDialog.h"

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TemplateDialog::TemplateDialog(QString const& _templateName, Session* _session) :
        session_(_session),
        ui_(new Ui::TemplateDialog)
      {
        ui_->setupUi(this);
        template_.reset(omni::proj::TemplateFactory::create(_templateName));
      
        ui_->preview->setSession(_session);
        ui_->preview->setTemplate(template_.get());
        ui_->parameters->setTemplate(template_.get());
      }

      TemplateDialog::~TemplateDialog()
      {
      }

      void TemplateDialog::replace()
      {
        if (!template_) return;

        session_->tunings().clear();
        
        close();
        emit replaceClicked();
      }

      void TemplateDialog::append()
      {
        if (!template_) return;
      }
    }
  }
}

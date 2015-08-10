#include <omni/ui/proj/MultiSetupDialog.h>

#include "ui_omni_ui_MultiSetupDialog.h"

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      MultiSetupDialog::MultiSetupDialog(QString const& _templateName, Session* _session) :
        QDialog(nullptr),
        session_(_session),
        ui_(new Ui::MultiSetupDialog)
      {
        ui_->setupUi(this);
        template_.reset(omni::proj::MultiSetupFactory::create(_templateName));
      
        ui_->preview->setSession(_session);
        ui_->preview->setMultiSetup(template_.get());
        ui_->parameters->setMultiSetup(template_.get());

        connect(ui_->parameters,SIGNAL(parametersUpdated()),ui_->preview,SLOT(updateProjectors()));
      }

      MultiSetupDialog::~MultiSetupDialog()
      {
      }

      void MultiSetupDialog::replace()
      {
        if (!template_) return;

        session_->tunings().clear();
        
        close();
        emit replaceClicked();
      }

      void MultiSetupDialog::append()
      {
        if (!template_) return;
      }
    }
  }
}

#include <omni/ui/proj/MultiSetupDialog.h>

#include "ui_omni_ui_MultiSetupDialog.h"

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      MultiSetupDialog::MultiSetupDialog(omni::proj::MultiSetup* _multiSetup, Session* _session) :
        QDialog(nullptr),
        session_(_session),
        multiSetup_(_multiSetup),
        ui_(new Ui::MultiSetupDialog)
      {
        ui_->setupUi(this);
        
        ui_->btnReplace->setVisible(!session_->tunings().empty());


        this->setWindowTitle("Multi Projector Setup");
        ui_->preview->setSession(_session);
        ui_->preview->setMultiSetup(multiSetup_);
        ui_->parameters->setMultiSetup(multiSetup_);

        ui_->groupSetup->setTitle(multiSetup_->getTypeId().str());

        connect(ui_->parameters,SIGNAL(parametersUpdated()),ui_->preview,SLOT(updateProjectors()));

        connect(ui_->btnCancel,SIGNAL(clicked()),this,SLOT(cancel()));
        connect(ui_->btnAppend,SIGNAL(clicked()),this,SLOT(append()));
        connect(ui_->btnReplace,SIGNAL(clicked()),this,SLOT(replace())); 
      }

      MultiSetupDialog::~MultiSetupDialog()
      {
      }
        
      MultiSetupDialog::Result MultiSetupDialog::open(omni::proj::MultiSetup* _multiSetup, Session* _session)
      {
        MultiSetupDialog _dialog(_multiSetup,_session);
        _dialog.exec();
        return _dialog.result_;
      }

      void MultiSetupDialog::replace()
      {
        result_ = multiSetup_ ? Result::REPLACE : Result::CANCELLED;
        accept();
      }

      void MultiSetupDialog::append()
      {
        result_ = multiSetup_ ? Result::APPEND : Result::CANCELLED;
        accept();
      }

      void MultiSetupDialog::cancel() 
      {
        result_ = Result::CANCELLED;
        reject();
      }
    }
  }
}

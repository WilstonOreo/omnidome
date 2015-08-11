#include <omni/ui/Export.h>

#include <QFileDialog>
#include <QMessageBox>
#include <omni/Session.h>
#include <omni/util.h>

#include "ui_omni_ui_Export.h"

namespace omni
{
  namespace ui
  {
    Export::Export(QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
      
      connect(ui_->btnExport,SIGNAL(clicked()),this,SLOT(exportToFile()));
    }
    
    Export::Export(Session* _session, QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
      setSession(_session);
    }

    Export::~Export()
    {
    }
      
    Session const* Export::session() const
    {
      return session_;
    }
  
    void Export::setSession(Session* _session)
    {
      session_=_session;
    }

    void Export::exportToFile()
    {
      RenderOptions _options = getRenderOptions();

      QString _filename = QFileDialog::getSaveFileName(this, "Save Calibration Image...", ".", "Calibration Images (*.png)");
      if( _filename.isEmpty() ) return;

      session_->renderToFile(_filename,_options);
      ui_->editExportFilename->setText(_filename);
      QMessageBox::information(this,"Session exported.",QString("Session has been exported to '%1'").arg(_filename));
    }

    omni::RenderOptions Export::getRenderOptions()
    {
      RenderOptions _renderOptions;

      return _renderOptions;
    }
  }
}

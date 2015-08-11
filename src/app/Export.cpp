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
      connect(ui_->btnExport,SIGNAL(clicked()),this,SLOT(exportToFile()));
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

      if (ui_->rbSepNone->isChecked())
      {
        _renderOptions.setSeparationMode(SeparationMode::NONE);
      } else
      if (ui_->rbSepScreens->isChecked())
      {
        _renderOptions.setSeparationMode(SeparationMode::SCREENS);
      } else
      if (ui_->rbSepProj->isChecked())
      {
        _renderOptions.
 setSeparationMode(SeparationMode::PROJECTORS);
      }

      if (ui_->rbUVWMap->isChecked())
      {
        _renderOptions.setMappingOutputMode(Mapping::OutputMode::UVW);
      } else
      if (ui_->rbTextureCoordinates->isChecked())
      {
        _renderOptions.setMappingOutputMode(Mapping::OutputMode::TEXCOORDS);
      } else
      if (ui_->rbMappedInput->isChecked())
      {
        _renderOptions.setMappingOutputMode(Mapping::OutputMode::MAPPED_INPUT);
      }
      return _renderOptions;
    }
  }
}

/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Export.h"

#include <QFileDialog>
#include <QMessageBox>
#include <omni/Session.h>
#include <omni/util.h>

#include "ui_omni_ui_Export.h"

namespace omni {
  namespace ui {
    Export::Export(QWidget *_parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      setup();
    }

    Export::~Export()
    {}

    void Export::exportToFile()
    {
      if (!dataModel()) return;

      QString _filename = QFileDialog::getSaveFileName(this,
                                                       "Save Calibration Image...",
                                                       ".",
                                                       "Calibration Images (*.png)");

      if (_filename.isEmpty()) return;

      dataModel()->renderToFile(_filename);
      ui_->editExportFilename->setText(_filename);
      QMessageBox::information(this, "Session exported.",
                               QString(
                                 "Session has been exported to '%1'").arg(
                                 _filename));
    }

    void Export::renderPreview() {
      ui_->outputPreview->render();
    }

    void Export::getExportSettings()
    {
      using namespace omni::render;
      auto& _exportSettings = dataModel()->exportSettings();

      _exportSettings.setExcludeUnassignedProjectors(
        ui_->chkExcludeNonSelectedScreens->isChecked());

      if (ui_->rbOmniCalibration->isChecked()) {
        _exportSettings.setOutputType(OutputType::OMNI_CALIBRATION);
      } else
      if (ui_->rbPlainImage->isChecked()) {
        _exportSettings.setOutputType(OutputType::PLAIN_IMAGE);
      }

      if (ui_->rbSepNone->isChecked())
      {
        _exportSettings.setSeparationMode(SeparationMode::NONE);
      } else
      if (ui_->rbSepScreens->isChecked())
      {
        _exportSettings.setSeparationMode(SeparationMode::SCREENS);
      } else
      if (ui_->rbSepProj->isChecked())
      {
        _exportSettings.setSeparationMode(SeparationMode::PROJECTORS);
      }

      if (ui_->rbUVWMap->isChecked())
      {
        _exportSettings.setMappingOutputMode(mapping::OutputMode::UVW);
      } else
      if (ui_->rbTextureCoordinates->isChecked())
      {
        _exportSettings.setMappingOutputMode(mapping::OutputMode::TEXCOORDS);
      } else
      if (ui_->rbMappedInput->isChecked())
      {
        _exportSettings.setMappingOutputMode(
          mapping::OutputMode::MAPPED_INPUT);
      }
    }

    void Export::dataToFrontend() {
      ui_->outputPreview->setDataModel(dataModel());

      if (dataModel()->mapping()) {
        bool _isUVW = dataModel()->mapping()->isUVW();
        ui_->rbUVWMap->setEnabled(_isUVW);

        if (!_isUVW && ui_->rbUVWMap->isChecked()) {
          ui_->rbTextureCoordinates->setChecked(true);
          ui_->rbUVWMap->setChecked(false);
        }
      }

      renderPreview();
    }

    void Export::setup() {
      ui_->setupUi(this);
      connect(ui_->btnExport, SIGNAL(clicked()), this,
              SLOT(exportToFile()));

      connect(ui_->rbOmniCalibration, SIGNAL(clicked(bool)), this,
              SLOT(selectOmniCalibration(bool)));
      connect(ui_->rbPlainImage, SIGNAL(clicked(bool)), this,
              SLOT(selectPlainImage(bool)));

      for (auto& _widget :
           std::vector<QWidget *>({ ui_->chkExcludeNonSelectedScreens,
                                    ui_->rbSepNone,
                                    ui_->rbSepScreens,
                                    ui_->rbSepProj,
                                    ui_->rbMappedInput,
                                    ui_->rbTextureCoordinates,
                                    ui_->rbUVWMap })) {
        connect(_widget, SIGNAL(clicked()), this, SLOT(renderPreview()));
        connect(_widget, SIGNAL(clicked()), this, SLOT(getExportSettings()));
      }

      selectOmniCalibration(true);
    }

    void Export::selectPlainImage(bool _selected) {
      ui_->lbSeparateOutput->setVisible(_selected);
      ui_->frameSeparation->setVisible(_selected);
      ui_->lbCalibrationDataType->setVisible(_selected);
      ui_->frameCalibrationDataType->setVisible(_selected);
      renderPreview();
    }

    void Export::selectOmniCalibration(bool _selected) {
      ui_->lbSeparateOutput->setVisible(!_selected);
      ui_->frameSeparation->setVisible(!_selected);
      ui_->lbCalibrationDataType->setVisible(!_selected);
      ui_->frameCalibrationDataType->setVisible(!_selected);
      renderPreview();
    }
  }
}

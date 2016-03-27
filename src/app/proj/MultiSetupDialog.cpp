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

#include "proj/MultiSetupDialog.h"

#include "ui_omni_ui_MultiSetupDialog.h"

namespace omni {
  namespace ui {
    namespace proj {
      MultiSetupDialog::MultiSetupDialog(omni::proj::MultiSetup *_multiSetup,
                                         std::shared_ptr<Session>_session) :
        QDialog(nullptr),
        session_(_session),
        multiSetup_(_multiSetup),
        ui_(new Ui::MultiSetupDialog)
      {
        ui_->setupUi(this);

        ui_->btnReplace->setVisible(!session_->tunings().empty());

        this->setWindowTitle("Multi Projector Setup");
        ui_->preview->setDataModel(_session);
        ui_->preview->setMultiSetup(multiSetup_);
        ui_->parameters->setMultiSetup(multiSetup_);

        ui_->groupSetup->setTitle(multiSetup_->getTypeId().str());

        connect(ui_->parameters, SIGNAL(parametersUpdated()), ui_->preview,
                SLOT(updateProjectors()));

        connect(ui_->btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
        connect(ui_->btnAppend, SIGNAL(clicked()), this, SLOT(append()));
        connect(ui_->btnReplace, SIGNAL(clicked()), this, SLOT(replace()));
      }

      MultiSetupDialog::~MultiSetupDialog()
      {}

      MultiSetupDialog::Result MultiSetupDialog::open(
        omni::proj::MultiSetup *_multiSetup,
        std::shared_ptr<Session>_session)
      {
        MultiSetupDialog _dialog(_multiSetup, _session);

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

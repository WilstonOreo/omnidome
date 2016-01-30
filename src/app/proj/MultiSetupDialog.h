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

/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_UI_EXPORT_H_
#define OMNI_UI_EXPORT_H_

#include <memory>
#include <QWidget>
#include <omni/Session.h>
#include "mixin/DataModel.h"

namespace omni {
  class Session;

  namespace ui {
    namespace Ui {
      class Export;
    }

    class Export :
      public QWidget,
      public mixin::SharedDataModel<Export,Session>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(Export,Session)
      public:
        Export(QWidget * = nullptr);
        ~Export() override;

      public slots:
        void exportToFile();
        void renderPreview();

        void selectPlainImage(bool);
        void selectOmniCalibration(bool);
        void getExportSettings();

      signals:
        void dataModelChanged();

      private:
        void dataToFrontend() override;
        inline bool frontendToData() override {
          return false;
        }

        void                 setup();

        std::unique_ptr<Ui::Export> ui_;
    };
  }
}

#endif /* OMNI_UI_EXPORT_H_ */

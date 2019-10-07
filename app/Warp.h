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

#ifndef OMNI_UI_WARP_H_
#define OMNI_UI_WARP_H_

#include <memory>
#include <omni/Session.h>
#include "DockWidget.h"
#include "mixin/DataModel.h"

namespace omni {
  class WarpGrid;

  namespace ui {
    namespace Ui {
      class Warp;
    }

    /// Dock widget for editing warp grid parameters
    class Warp :
      public DockWidget,
      public mixin::SharedDataModel<Warp,Session>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(Warp,Session)

      public:
        Warp(QWidget * = nullptr);
        ~Warp() override;

      public slots:
        void resetWarpGrid();
        void resizeWarpGrid(bool);
        void changeInterpolation(int);

      signals:
        void dataModelChanged();

      private:
        /// Update sliders from current warp grid
        void                  dataToFrontend() override;

        /// Assign slider values to current warp grid
        bool                  frontendToData() override;

        omni::WarpGrid const* warpGrid() const;
        omni::WarpGrid      * warpGrid();

        std::unique_ptr<Ui::Warp> ui_;
    };
  }
}

#endif /* OMNI_UI_WARP_H_ */

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

#ifndef OMNI_UI_PROJ_MULTISETUPPREVIEW_H
#define OMNI_UI_PROJ_MULTISETUPPREVIEW_H

#include <omni/ui/GLView3D.h>
#include <omni/proj/MultiSetup.h>

namespace omni {
  namespace ui {
    namespace proj {
      /// Preview for multisetup
      class MultiSetupPreview : public GLView3D {
          Q_OBJECT
        public:
          MultiSetupPreview(QWidget *_parent);
          ~MultiSetupPreview();

          /// Pointer to multi setup
          omni::proj::MultiSetup      * multiSetup();

          /// Pointer to multi setup (const version)
          omni::proj::MultiSetup const* multiSetup() const;
          void                          setMultiSetup(
            omni::proj::MultiSetup *_template);

        public slots:
          /// Update projectors
          void updateProjectors();

        protected:
          void paintGL();

        private:
          omni::proj::MultiSetup *multiSetup_ = nullptr;

          std::vector<omni::proj::Projector> projectors_;
          std::list<visual::Projector> projectorViz_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUPPREVIEW_H */

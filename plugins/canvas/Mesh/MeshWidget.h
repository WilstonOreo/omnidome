/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
 #ifndef OMNI_UI_CANVAS_MESH_H_
 #define OMNI_UI_CANVAS_MESH_H_

#include <memory>
#include <QPushButton>
#include <QCheckBox>
#include <omni/ui/CanvasParameters.h>
#include "Mesh.h"

namespace omni {
  namespace ui {
    namespace canvas {
      /// Widget for mesh canvas
      class Mesh : public ui::CanvasParameters {
        Q_OBJECT

        public:
          typedef omni::canvas::Mesh canvas_type;
          Mesh(QWidget * = nullptr);
          ~Mesh();

          canvas_type      * mesh();
          canvas_type const* mesh() const;

        public slots:
          /// Display open file dialog to load mesh
          void loadMesh();
          void centerMesh(bool);
          void scaleMesh(bool);

        private:
          void dataToFrontend();

          /// Return true if data has changed by front end
          bool frontendToData();

          QUniquePtr<QPushButton> btnLoad_;
          QUniquePtr<QCheckBox> chkCenter_;
          QUniquePtr<QCheckBox> chkScale_;
          QString lastDir_;
      };
    }
  }
}

#endif /* OMNI_UI_CANVAS_MESH_H_ */

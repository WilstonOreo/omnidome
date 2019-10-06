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

#ifndef OMNI_CANVAS_MESH_H_
#define OMNI_CANVAS_MESH_H_

#include <QObject>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <omni/canvas/Interface.h>
#include <omni/media/Location.h>
#include <omni/visual/VBO.h>
#include <omni/geometry/Vertex.h>

namespace omni {
  namespace canvas {
    typedef OpenMesh::PolyMesh_ArrayKernelT<>PolygonMesh;

    class Mesh : public QObject, public canvas::Interface {
        Q_OBJECT
        Q_INTERFACES(omni::canvas::Interface)
        Q_PLUGIN_METADATA(IID OMNI_CANVAS_INTERFACE_IID)
        OMNI_PLUGIN_INFO("Custom Mesh canvas",
                         "Copyright (C) 2016")

      public:
        OMNI_REGISTER_CLASS(Factory, Mesh)

        Mesh();
        ~Mesh() final = default;

        void                  draw() const override;

        void                  update() override;

        /// Center To Scene
        void                  setCenterToScene(bool);
        bool                  centerToScene() const;

        /// Scele To Scene
        void                  setScaleToScene(bool);
        bool                  scaleToScene() const;

        /// Return mesh bounding box
        Box                   bounds() const override;

        QVector3D             center() const override;

        /// Make new box parameter widget
        ui::CanvasParameters* widget() override;

        /// Load mesh from file
        void                  load(QString const& _filename);

        void                  fromPropertyMap(PropertyMap const&) override;
        void                  toPropertyMap(PropertyMap&) const override;

      private:
        void                  updateBoundingBox();

        void                  updateBoundingBox(std::vector<Vertex> const& _vertices);

        size_t vertexCount_ = 0;
        PolygonMesh mesh_;
        omni::Box   bounds_;
        visual::VBO vbo_;
        media::Location meshFile_;
        QString lastDir_;
        bool centerToScene_ = true;
        bool scaleToScene_ = true;
    };
  }

  using canvas::PolygonMesh;
}

#endif /* OMNI_CANVAS_MESH_H_ */

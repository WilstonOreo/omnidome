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

#ifndef OMNI_VISUAL_WARPGRID_H_
#define OMNI_VISUAL_WARPGRID_H_

#include <memory>
#include <omni/WarpGrid.h>
#include <omni/geometry/Vertex2D.h>
#include <omni/visual/Interface.h>
#include <omni/visual/Circle.h>
#include <omni/visual/VBO.h>


namespace omni {
  namespace visual {
    /// Draw Handler for WarpGrid
    class OMNI_EXPORT WarpGrid : public Interface {
      public:
        WarpGrid(omni::WarpGrid const& _warpGrid);
        ~WarpGrid();

        /// Draw warp grid
        void draw() const;

        /// Draw lines for warp grid
        void drawLines();

        /// Draw handles for warp grid
        void drawHandles(QColor const& _color,
                         QRectF const& _rect);

        /// Generate geometry
        void update();

        /// Return number of subdivisions
        int  subdivisions() const;

        /// Set number of subdivisions
        void setSubdivisions(int _subDiv);

      private:
        std::unique_ptr<Circle> circle_;
        std::vector<Vertex2D>   vertices_;
        std::vector<QVector2D>  gridVertices_;
        std::vector<GLuint> indices_;
        std::vector<GLuint> gridIndices_;
        VBO vertexVbo_, indexVbo_;
        VBO gridVertexVbo_, gridIndexVbo_;

        omni::WarpGrid const& warpGrid_;
        int subdivisions_ = 8;
    };
  }
}

#endif /* OMNI_VISUAL_WARPGRID_H_ */

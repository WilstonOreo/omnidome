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
#ifndef OMNI_VISUAL_PLANE_H_
#define OMNI_VISUAL_PLANE_H_

#include <functional>
#include <omni/visual/VertexVBO.h>
#include <omni/visual/Interface.h>
#include <omni/visual/ContextBoundPtr.h>

namespace omni {
  namespace visual {
    /**@brief A plane with n x m grid points
       @detail Vertex data is not stored on host, it is copied to GPU directly.
               Plane has dimensions [-0.5,-0.5] - [0.5,0.5]
     **/
    class OMNI_EXPORT Plane : public visual::Interface {
      public:
        typedef std::function<void (QVector2D&, size_t, size_t)>
          texcoord_transform_functor_type;

        Plane();
        ~Plane();

        /// Draw plane
        void   draw() const;

        /// Update mesh with texcoord transformation
        void   update(texcoord_transform_functor_type _f);

        /// Update mesh (without transforming texture coordinates)
        void   update();

        /// Return number of horizontal subdivisions
        size_t horizontal() const;

        /// Set horizontal subdivisions (does not regenerate the mesh)
        void   setHorizontal(size_t _horizontal);

        /// Return number of vertical subdivisions
        size_t vertical() const;

        /// Set vertical subdivisions (does not regenerate the mesh)
        void   setVertical(size_t _vertical);

        /// Set horizontal and vertical subdivisions (does not regenerate the
        // mesh)
        void   resize(size_t _horizontal,
                      size_t _vertical);

      private:
        size_t horizontal_ = 10;
        size_t vertical_   = 10;
        ContextBoundPtr<VertexVBO> vbo_;
    };
  }
}

#endif /* OMNI_VISUAL_PLANE_H_ */

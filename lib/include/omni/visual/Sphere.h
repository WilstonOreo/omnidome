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

#ifndef OMNI_VISUAL_SPHERE_H_
#define OMNI_VISUAL_SPHERE_H_

#include <omni/visual/Interface.h>
#include <omni/visual/VertexVBO.h>

namespace omni
{
  namespace visual
  {
    /// Draw Handler for visualizing a centered sphere
    class OMNI_EXPORT Sphere : public Interface
    {
    public:
      enum TexCoordsMode {
            EQUIRECTANGULAR,
            FISHEYE
      };

      Sphere(qreal _radius = 1.0);
      ~Sphere();

      /// Return radius of sphere
      qreal radius() const;

      /// Set new sphere radius
      void setRadius(qreal);

      /// Return number of stacks (in Z direction)
      int stacks() const;

      /// Set number of stacks
      void setStacks(int _stacks);

      /// Return slices of sphere
      int slices() const;

      /// Set number of slices and update mesh
      void setSlices(int _slices);

      /// Return top position where to cut off sphere
      float top() const;

      /// Set new top position where to cut off sphere
      void setTop(float);

      /// Return bottom position where to cut off sphere
      float bottom() const;

      /// Set new bottom position where to cut off sphere
      void setBottom(float);

      /// Return tex coords mode; fisheye or equirectangular
      TexCoordsMode texCoordsMode() const;

      /// Set tex coords mode, fisheye or equirectangular
      void setTexCoordsMode(TexCoordsMode);

      /// Draws sphere from vertex buffer object
      void draw() const;

      /// Regenerates the mesh and updates the vertex buffer objects
      void update();

    private:
      void generateStack(
          float _top, float _bottom,
          float _topRadius, float _bottomRadius);

      int stacks_ = 64;
      int slices_ = 128;

      float top_ = 1.0;
      float bottom_ = -1.0;

      qreal radius_ = 1.0;

      VertexVBO vbo_;
      VertexVBO::vertex_buffer_type vertices_;
      VertexVBO::index_buffer_type indices_;
      TexCoordsMode texCoordsMode_ = EQUIRECTANGULAR;
    };
  }
}

#endif /* OMNI_VISUAL_SPHERE_H_ */

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
#ifndef OMNI_VISUAL_VERTEXVBO_H_
#define OMNI_VISUAL_VERTEXVBO_H_

#include <vector>
#include <omni/visual/VBO.h>
#include <omni/visual/ContextBoundPtr.h>
#include <omni/geometry/Vertex.h>

namespace omni {
  namespace visual {
    /// Utility class for storing and drawing VBO data
    class OMNI_EXPORT VertexVBO {
      public:
        /// Vertex buffer type
        typedef std::vector<Vertex>vertex_buffer_type;

        /// Index buffer type
        typedef std::vector<uint32_t>index_buffer_type;

        VertexVBO();

        VertexVBO(vertex_buffer_type const& _vertices,
        index_buffer_type const& _indices);


        /// Copy buffers to GPU
        void buffer(
          vertex_buffer_type const& _vertices,
          index_buffer_type const& _indices);

        /// Bind buffer for drawing
        void bind() const;

        /// Draw buffer as triangles
        void draw() const;

        /// Draw specific indices from buffer as triangles
        void draw(uint32_t _numIndices,
                  GLuint _drawType = GL_TRIANGLES) const;

        /// Unbind buffer
        void unbind() const;

        template<typename ... ARGS>
        void bindAndDraw(ARGS&& ... _args) const {
          bind();
          draw(_args ...);
          unbind();
        }

        /// Return number of indices in this buffer
        size_t numIndices() const;

        /// Number of triangles = number of indices / 3
        inline size_t numTriangles() const {
          return numIndices() / 3;
        }

        /// Return vertex vertex buffer object
        VBO const& vertexVbo() const;

        /// Return index vertex buffer object
        VBO const& indexVbo() const;

      private:
        size_t numIndices_ = 0;
        std::unique_ptr<VBO>    vertexVbo_, indexVbo_;
    };
  }
}


#endif /* OMNI_VISUAL_VERTEXVBO_H_ */

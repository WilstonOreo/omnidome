/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#include <omni/visual/VertexVBO.h>
#include <omni/visual/util.h>

namespace omni {
    namespace visual {

        void VertexVBO::buffer(vertex_buffer_type const& _vertices, index_buffer_type const& _indices) {
            vertexVbo_.freeAndGen();
            indexVbo_.freeAndGen();

            with_current_context([&](QOpenGLFunctions& _)
            {
                // bind VBO in order to use
                _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());
                {
                    _.glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
                }
                _.glBindBuffer(GL_ARRAY_BUFFER,0);

                _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo_.id());
                {
                    _.glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
                }
                _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            });
            numIndices_ = _indices.size();
        }

        void VertexVBO::bind() const {
            with_current_context([this](QOpenGLFunctions& _)
            {
                _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());
                _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo_.id());

                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glEnableClientState(GL_VERTEX_ARRAY);
            });

        }

        void VertexVBO::draw() const {
            draw(numIndices_);
        }

        void VertexVBO::draw(uint32_t _numElements, GLuint _drawType) const {
            if (_numElements < 4 || _numElements > numIndices_) {
                return;
            }
            with_current_context([&](QOpenGLFunctions& _)
            {
                glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex),(void*)Vertex::texCoordOffset());
                glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)Vertex::normalOffset());
                glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)Vertex::posOffset());
                _.glDrawElements(_drawType,_numElements-4,GL_UNSIGNED_INT,0);
            });
        }

        void VertexVBO::unbind() const {
            with_current_context([this](QOpenGLFunctions& _)
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                _.glBindBuffer(GL_ARRAY_BUFFER, 0 );
                _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            });
        }

        size_t VertexVBO::numIndices() const {
            return numIndices_;
        }

        VBO const& VertexVBO::vertexVbo() const {
            return vertexVbo_;
        }

        VBO const& VertexVBO::indexVbo() const {
            return indexVbo_;
        }
    }
}

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

#ifndef OMNI_VISUAL_VBO_H
#define OMNI_VISUAL_VBO_H

#include <QOpenGLContext>
#include <omni/visual/util.h>

namespace omni {
  namespace visual {
    /// A small Vertex Buffer Object class
    struct OMNI_EXPORT VBO
    {
      VBO();

      /// Destructor. Frees VBO if necessary
      ~VBO();

      /// Generate a new VBO and get the associated ID
      void gen();

      /// Free the existing buffer generates a new one
      void freeAndGen();

      /// Copy static vertex array onto GPU
      template<typename BUF>
      void bufferStaticArray(BUF const& _buf) {
        withCurrentContext([&](QOpenGLFunctions& _) {
          _.glBindBuffer(GL_ARRAY_BUFFER, id());
          {
            _.glBufferData(GL_ARRAY_BUFFER,
                           _buf.size() * sizeof(typename BUF::value_type),
                           _buf.data(),
                           GL_STATIC_DRAW);
          }
          _.glBindBuffer(GL_ARRAY_BUFFER, 0);
        });
      }

      /// Copy static index array onto GPU
      template<typename BUF>
      void bufferStaticElementArray(BUF const& _buf) {
        withCurrentContext([&](QOpenGLFunctions& _) {
          _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id());
          {
            _.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                           _buf.size() * sizeof(typename BUF::value_type),
                           _buf.data(),
                           GL_STATIC_DRAW);
          }
          _.glBindBuffer(GL_ARRAY_BUFFER, 0);
        });
      }

      /// Free VBO via glDeleteBuffers
      void   free();

      /// Return buffer id
      GLuint id() const;

      private:
        GLuint id_ = 0;
    };
  }
}

#endif /* OMNI_VISUAL_VBO_H */

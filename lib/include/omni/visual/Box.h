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
#ifndef OMNI_VISUAL_BOX_H_
#define OMNI_VISUAL_BOX_H_

#include <omni/geometry/Box.h>
#include <omni/visual/Plane.h>

namespace omni {
  namespace visual {
    /**@brief Box with subdivisions for drawing
    **/
    class OMNI_EXPORT Box : public Interface {
      public:
        Box();
        ~Box();

        /// Draw box
        void   draw() const;

        /// Update box mesh
        void   update();

      private:
        size_t horizontal_ = 4;
        size_t vertical_   =  4;
        VertexVBO vbo_;
    };
  }
}

#endif /* OMNI_VISUAL_BOX_H_ */

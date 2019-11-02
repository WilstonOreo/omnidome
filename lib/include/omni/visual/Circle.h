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

#ifndef OMNI_VISUAL_CIRCLE_H_
#define OMNI_VISUAL_CIRCLE_H_

#include <array>
#include <omni/visual/VBO.h>

namespace omni {
  namespace visual {
    /// Visualizer for drawing a circle with VBO
    class OMNI_EXPORT Circle {
      public:
        Circle();

        /// Standard number of segments
        static constexpr std::size_t numberSegments = 32;

        /// Draw lined circle
        void drawLine(QPointF const& _pos,
                      float _rX,
                      float _rY) const;

        /// Draw filled circle with triangle fan
        void drawFill(QPointF const& _pos,
                      float _rX,
                      float _rY) const;

        /// Update circle mesh
        void update();

      private:
        VBO vertexVbo_;
        std::array<QVector2D, numberSegments + 2> vertices_;
    };
  }
}


#endif /* OMNI_VISUAL_CIRCLE_H_ */

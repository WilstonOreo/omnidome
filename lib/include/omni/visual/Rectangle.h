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

#ifndef OMNI_VISUAL_RECTANGLE_H_
#define OMNI_VISUAL_RECTANGLE_H_

#include <QRectF>
#include <omni/global.h>

namespace omni {
  namespace visual {
    /// Visualizer helper class for drawing a rectangle in an OpenGL context
    struct OMNI_EXPORT Rectangle
    {
      /// Draw rectangle from border coordinates
      static void draw(float _left = -0.5,
                       float _right = 0.5,
                       float _top = 0.5,
                       float _bottom = -0.5);

      static void draw(QSize const& _size);

      /// Draw rectangle from given QRectF
      static void draw(QRectF const&);

      /// Draw rectangle from border coordinates, flipped on y axis version
      static void drawFlipped(float _left = -0.5,
                              float _right = 0.5,
                              float _top = 0.5,
                              float _bottom = -0.5);

      /// Draw rectangle from given QRectF, flipped on y axis version
      static void drawFlipped(QRectF const&);
    };
  }
}

#endif /* OMNI_VISUAL_RECTANGLE_H_ */

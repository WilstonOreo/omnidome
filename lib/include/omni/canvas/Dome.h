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

#ifndef OMNI_CANVAS_DOME_H_
#define OMNI_CANVAS_DOME_H_

#include <omni/geometry/Vertex.h>
#include <omni/visual/Sphere.h>
#include <omni/canvas/Envelope.h>

namespace omni {
  namespace canvas {
    /// A basic dome canvas width a radius/diameter and offset
    class OMNI_EXPORT Dome : public Envelope {
      public:

        Dome();
        virtual ~Dome();

        /// Return radius (10.0m by default)
        qreal radius() const;

        /// Set radius in meter
        void setRadius(qreal);

        /// Return diameter in meter
        qreal diameter() const;

        /// Set diameter in meter
        void setDiameter(qreal);

        /// Return internally cached bounding box
        Box          bounds() const;

        /// Draw dome sphere
        virtual void draw() const;

        /// Draw auxiliary elements
        virtual void drawAux() const;

        /// Update dome mesh
        virtual void update();

        /// Serialize to stream
        virtual void toPropertyMap(PropertyMap&) const;

        /// Deserialize from stream
        virtual void fromPropertyMap(PropertyMap const&);

      protected:
        /// Sphere visualizer
        visual::Sphere sphere_;
    };
  }
}

#endif /* OMNI_CANVAS_DOME_H_ */

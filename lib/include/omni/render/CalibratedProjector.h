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

#ifndef OMNI_RENDER_CALIBRATEDPROJECTOR_H_
#define OMNI_RENDER_CALIBRATEDPROJECTOR_H_

#include <QRect>
#include <omni/RenderBuffer.h>
#include <omni/proj/ColorCorrection.h>

namespace omni {
  namespace render {
    class OMNI_EXPORT CalibratedProjector {
      private:
        CalibratedProjector(proj::Tuning const *);

        QImage              toImage();

        RenderBuffer const& buffer() const;
        QRect const       & screenGeometry() const;
        ColorCorrection     colorCorrection() const;
        bool                virtualScreen() const;

      public:
        template<typename PROJECTION, typename MODELVIEW>
        static void renderToBuffer(RenderBuffer& _buffer,
                                   PROJECTION _proj,
                                   MODELVIEW _mv);

        bool  virtualScreen_;
        QRect screenGeometry_;
        ColorCorrection colorCorrection_;
        RenderBuffer    buffer_;
    };
  }
}

#endif /* OMNI_RENDER_CALIBRATEDPROJECTOR_H_ */

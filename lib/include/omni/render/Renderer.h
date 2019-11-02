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

#ifndef OMNI_RENDER_RENDERER_H_
#define OMNI_RENDER_RENDERER_H_

#include <map>
#include <QScreen>
#include <QImage>
#include <omni/proj/ColorCorrection.h>

namespace omni {
  class Session;
  namespace proj {
    class Tuning;
  }

  namespace render {
    class OMNI_EXPORT Renderer {
      public:
        Renderer(Session const& _session);

        void renderToFile(QString const& _filename);

        /// Render to omni calibration (OmniC) file
        void renderOmniCalibration(QString const& _filename);

        std::map<QScreen const *, QImage>
             stitchScreens(std::vector<const proj::Tuning *>const&) const;

      private:
        Session const& session_;
    };
  }
}

#endif /* OMNI_RENDER_RENDERER_H_ */
